package com.lai.whiteboard.util

import android.R.attr
import android.graphics.Matrix
import android.graphics.PointF
import android.util.Log
import android.view.MotionEvent
import androidx.core.math.MathUtils
import java.lang.Math.max
import kotlin.math.*


/**
 *
 * @author  Lai
 *
 * @time 2021/1/10 1:16
 * @describe describe
 *
 */
object PointUtils {

    var pointsPerLengthSum = 5

    fun isCenter(centerPoint: PointF, fromPoint: PointF, toPoint: PointF): Boolean {
        val isCenterX = abs((fromPoint.x + toPoint.x) / 2.0 - centerPoint.x) < 0.0001
        val isCenterY = abs((fromPoint.y + toPoint.y) / 2.0 - centerPoint.y) < 0.0001
        return isCenterX && isCenterY
    }


    fun line(point1: PointF, point2: PointF): ArrayList<PointF> {
        var steep = false
        val list = ArrayList<PointF>();

        point1.set(0f, 0f)
        point2.set(400f, 400f)


        var x0 = point1.x.toInt()
        var y0 = point1.y.toInt()
        var x1 = point2.x.toInt()
        var y1 = point2.y.toInt()

        if (abs(x0 - x1) < abs(y0 - y1)) {
            x0 = y0.also { y0 = x0 }
            x1 = y1.also { y1 = x1 }
            steep = true
        }

        if (x0 > x1) {
            x0 = x1.also { x1 = x0 }
            y0 = y1.also { y1 = y0 }
        }

        val dx = x1 - x0;
        val dy = y1 - y0;
        val derror2 = abs(dy) * 2;
        var error2 = 0
        var y = y0

        for (x in x0..x1) {
            if (steep) {
                list.add(PointF(y * 1f, attr.x * 1f))
            } else {
                list.add(PointF(attr.x * 1f, y * 1f))
            }
            error2 += derror2
            if (error2 > dx) {
                y += if (y1 > y0) 1 else -1
                error2 -= dx * 2
            }
        }

        return list
    }


    fun createBesselCurvePoints2(
        from: PointF,
        to: PointF,
        control: PointF,
        pointSize: Int = 40
    ): ArrayList<PointF> {


        val list = ArrayList<PointF>();
        var t = 0.0

        while (t <= 1.0) {
            val x = (1f - t) * (1f - t) * from.x + 2f * (1f - t) * t * control.x + t * t * to.x;
            val y = (1f - t) * (1f - t) * from.y + 2f * (1f - t) * t * control.y + t * t * to.y;
            list.add(PointF(x.toFloat(), y.toFloat()))
            t += 0.05
        }
        return list
    }



    fun pointsWithFrom(
        from: PointF,
        to: PointF,
        control: PointF,
        pointSize: Float? = 15f
    ): ArrayList<PointF> {
        val P0 = from;
        // 如果 control 是 from 和 to 的中点，则将 control 设置为和 from 重合
        val P1 = if (this.isCenter(control, from, to)) {
            from
        } else {
            control
        }
        //val P1 = control
        val P2 = to

        val ax = P0.x - 2 * P1.x + P2.x;
        val ay = P0.y - 2 * P1.y + P2.y;
        val bx = 2 * P1.x - 2 * P0.x;
        val by = 2 * P1.y - 2 * P0.y;

        val A = 4 * (ax * ax + ay * ay);
        val B = 4 * (ax * bx + ay * by);
        val C = bx * bx + by * by;

        val totalLength = lengthWithT(1.0, A.toDouble(), B.toDouble(), C.toDouble());  // 整条曲线的长度
        val pointsPerLength = pointsPerLengthSum / (pointSize?:15f) // 用点的尺寸计算出，单位长度需要多少个点
        val count = max(1.0, ceil(pointsPerLength * totalLength)).toInt()  // 曲线应该生成的点数
        //Log.e("11111", "pointsPerLength : $pointsPerLength")
        val list = ArrayList<PointF>()
        /*if (count == 1) {
            list.add(PointF(to.x, to.y))
            return list
        }*/
        for (i in 0 until count) {
            var t = i * 1.0 / count
            val length = t * totalLength

            t = tWithT(t, length, A.toDouble(), B.toDouble(), C.toDouble())
            // 根据 t 求出坐标
            val x = (1 - t) * (1 - t) * P0.x + 2 * (1 - t) * t * P1.x + t * t * P2.x
            val y = (1 - t) * (1 - t) * P0.y + 2 * (1 - t) * t * P1.y + t * t * P2.y
            list.add(PointF(x.toFloat(), y.toFloat()))
        }
        // Log.e("11111", "list size : ${list.size}")
        return list
    }


    /**
    长度函数反函数，根据 length，求出对应的 t，使用牛顿切线法求解

    @param t 给出的近似的 t，比如求长度占弧长 0.3 的 t，t 应该是接近 0.3，则传入近似值 0.3
    @param length 目标弧长，实际长度，非占比
    @param A 见【注意】
    @param B 见【注意】
    @param C 见【注意】
    @return 结果 t 值
     */
    fun tWithT(t: Double, length: Double, A: Double, B: Double, C: Double): Double {
        var t1 = t
        var t2: Double
        var lastDouble = 0.0
        while (true) {
            val speed = speedWithT(t, A, B, C)
            if (speed < 0.0001) {
                t2 = t1
                break
            }
            t2 = t1 - (lengthWithT(t1, A, B, C) - length) / speed
            if (abs(t1 - t2) < 0.0001) {
                break
            }
            //02-04 23:21:59.644 20094-20094/? E/11111: t1 0.501487692151914 t2 0.4985078813922082

            //Log.e("11111", "t1 $t1 t2 $t2")
            if (lastDouble == t1) {
                break
            }

            lastDouble = t2
            t1 = t2


        }
        return t2
    }


    /**
     * 根据两点计算方向角度
     * @param startx
     * @param starty
     * @param endx
     * @param endy
     * @return
     */
    fun calulateXYAnagle(
        startx: Float, starty: Float, endx: Float,
        endy: Float
    ): Float {
        val tan = (atan(abs((endy - starty) / (endx - startx))) * 180 / Math.PI).toFloat()
        return if (endx > startx && endy > starty)
        // 第一象限
        {
            -tan
        } else if (endx > startx && endy < starty)
        // 第二象限
        {
            tan
        } else if (endx < startx && endy > starty)
        // 第三象限
        {
            tan - 180
        } else {
            180 - tan
        }
    }


    /**
    速度函数 s(t) = sqrt(A * t^2 + B * t + C)
    @param t t 值
    @param A 见【注意】
    @param B 见【注意】
    @param C 见【注意】
    @return 贝塞尔曲线某一点的速度
     */
    fun speedWithT(t: Double, A: Double, B: Double, C: Double): Double {
        return sqrt((A * t.pow(2.0) + B * t + C).coerceAtLeast(0.0))
    }


    /**
    长度函数
    @param t t 值
    @param A 见【注意】
    @param B 见【注意】
    @param C 见【注意】
    @return t 值对应的曲线长度
     */
    fun lengthWithT(t: Double, A: Double, B: Double, C: Double): Double {
        if (A < 0.00001f) {
            return 0.0
        }
        val temp1 = sqrt(C + t * (B + A * t))
        val temp2 = (2 * A * t * temp1 + B * (temp1 - sqrt(C)));
        val temp3 = Math.log(abs(B + 2 * sqrt(A) * sqrt(C) + 0.0001))
        val temp4 = Math.log(abs(B + 2 * A * t + 2 * sqrt(A) * temp1) + 0.0001)
        val temp5 = 2 * sqrt(A) * temp2;
        val temp6 = (B * B - 4 * A * C) * (temp3 - temp4);

        return (temp5 + temp6) / (8 * A.pow(1.5))
    }

    fun createBesselCurvePoints(
        from: PointF,
        to: PointF,
        control: PointF,
        pointSize: Float = 40f
    ): ArrayList<PointF> {
        val list = ArrayList<PointF>();
        val segmentDistance = 1 //设置线段的距离为1px.

        val distance: Float = getDistanceTo(from, to) // 计算两个中间点的距离
        var numberOfSegments =
            48.0.coerceAtMost(floor(distance / segmentDistance.toDouble()).coerceAtLeast(24.0))
                .toInt() // 计算可以分成多少段
        var t = 0.0f

        val step = 1.0f / numberOfSegments.toFloat()
        for (j in 0 until numberOfSegments) {
            val x = (1f - t) * (1f - t) * from.x + 2f * (1f - t) * t * control.x + t * t * to.x;
            val y = (1f - t) * (1f - t) * from.y + 2f * (1f - t) * t * control.y + t * t * to.y;
            list.add(PointF(x, y))
            t += step
        }

        return list
    }


    private fun smoothPoint(
        midPoint1: PointF,
        midPoint2: PointF,
        prev1: PointF,
        t: Float
    ): PointF {
        val a1 = Math.pow(1.0f - t.toDouble(), 2.0)
        val a2 = (2.0f * (1.0f - t) * t)
        val a3 = t * t
        return PointF(
            (midPoint1.x * a1 + prev1.x * a2 + midPoint2.x * a3).toFloat(),
            (midPoint1.y * a1 + prev1.y * a2 + midPoint2.y * a3).toFloat()
        )
    }

    fun getDistanceTo(curPoint: PointF, point: PointF): Float {

        val x = curPoint.x - point.x
        val y = curPoint.y - point.y
        return sqrt(x * x + y * y.toDouble()).toFloat()


        /* return sqrt(
             (curPoint.x * 1.0 - point.x * 1.0).pow(2.0) + (curPoint.y * 1.0 - point.y * 1.0).pow(2.0)
         ).toFloat()*/
    }


    /**
     * As meaning of method name.
     * 获得两点之间的距离
     * @param p0
     * @param p1
     * @return
     */
    fun getDistanceBetween2Points(p0: PointF, p1: PointF): Float {
        return Math.sqrt(
            Math.pow(
                p0.y - p1.y.toDouble(),
                2.0
            ) + Math.pow(p0.x - p1.x.toDouble(), 2.0)
        ).toFloat()
    }

    fun getDistanceTo(
        currFloatX: Float,
        currFloatY: Float,
        toFloatX: Float,
        toFloatY: Float
    ): Float {
        return sqrt(
            (currFloatX * 1.0 - toFloatX * 1.0).pow(2.0) + (currFloatY * 1.0 - toFloatY * 1.0).pow(
                2.0
            )
        ).toFloat()
    }

    // 归一化顶点坐标
    fun vertexWithPoint(x: Float, y: Float, frameWidth: Int, frameHeight: Int): FloatArray {
        val openglX = (x / frameWidth.toFloat()) * 2f - 1f
        val openglY = 1f - (y / frameHeight.toFloat()) * 2f

        return floatArrayOf(MathUtils.clamp(openglX, -1f, 1f), MathUtils.clamp(openglY, -1f, 1f))
    }

    // opengl 偏移量
    fun offsetWithMatrixValue(
        x: Float,
        y: Float,
        frameWidth: Int,
        frameHeight: Int
    ): FloatArray {
        val openglX = (x / frameWidth.toFloat()) * 2f
        val openglY = -(y / frameHeight.toFloat()) * 2f
        return floatArrayOf(openglX, openglY)
    }

    fun multiplySum(
        point: PointF,
        point2: PointF,
        scalar: Float
    ): PointF {
        return PointF(
            (point.x + point2.x) * scalar,
            (point.y + point2.y) * scalar
        )
    }

    /**
     * 计算两个手指间的距离
     *
     * @param event 触摸事件
     * @return 放回两个手指之间的距离
     */
    fun distance(event: MotionEvent): Float {
        val x = event.getX(0) - event.getX(1)
        val y = event.getY(0) - event.getY(1)
        return sqrt(x * x + y * y.toDouble()).toFloat() //两点间距离公式
    }


    fun matrixToOpenglMatrix4f(matrix: Matrix, width: Int, height: Int): FloatArray {
        val array = FloatArray(9)
        matrix.getValues(array)
        val vertexWithPoint =
            offsetWithMatrixValue(array[Matrix.MTRANS_X], array[Matrix.MTRANS_Y], width, height)
        array[Matrix.MTRANS_X] = vertexWithPoint[0]
        array[Matrix.MTRANS_Y] = vertexWithPoint[1]


        /*val vertexWithPoint2 =
            offsetWithMatrixValue(array[Matrix.MSCALE_X], array[Matrix.MSCALE_Y], width, height)
        array[Matrix.MSCALE_X] = vertexWithPoint2[0]
        array[Matrix.MSCALE_Y] = vertexWithPoint2[1]*/

        // val matrix2 = Matrix()
        // matrix2.postTranslate(0.5f,0.6f)
        // matrix2.postScale(0.1f,0.2f)
        //matrix2.postRotate(45f,width/2f,height/2f)

        //Log.e("11111", "matrix2.toString() $matrix")
        /*

        glm::mat4(1.0f) mat4x4(

        (1.000000, 0.000000, 0.000000, 0.000000)
        (0.000000, 1.000000, 0.000000, 0.000000)
        (0.000000, 0.000000, 1.000000, 0.000000)
        (0.000000, 0.000000, 0.000000, 1.000000)



((0.100000, 0.000000, 0.000000, 0.000000),
 (0.000000, 0.200000, 0.000000, 0.000000),
 (0.000000, 0.000000, 0.000000, 0.000000),
  (0.000000, 0.000000, 0.000000, 1.000000))

   glm::mat4 myMatrix = glm::translate(glm::mat4(), glm::vec3(0.5f, 0.6f, 0.0f));
    //我们认为的
   (1.000000, 0.000000, 0.000000, 0.500000)
   (0.000000, 1.000000, 0.000000, 0.600000)
   (0.000000, 0.000000, 1.000000, 0.000000)
   (0.000000, 0.000000, 0.000000, 1.000000)
    //其实是这样的
   (1.000000, 0.000000, 0.000000, 0.000000)
   (0.000000, 1.000000, 0.000000, 0.000000)
   (0.000000, 0.000000, 1.000000, 0.000000)
   (0.500000, 0.600000, 0.000000, 1.000000)

       glm::mat4 myMatrix = glm::translate(glm::mat4(), glm::vec3(0.5f, 0.6f, 0.0f));
      mMatrix = glm::scale(mMatrix,glm::vec3(0.1f, 0.2f, 0.0f));
      mMatrix  = glm::rotate(mMatrix, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));

        (0.070711, 0.141421, 0.000000, 0.000000)
        (-0.070711, 0.141421, 0.000000, 0.000000)
        (0.000000, 0.000000, 0.000000, 0.000000)
        (0.000000, 0.000000, 0.000000, 1.000000)

        {[0.07071068, -0.14142136, 811.4795]
        [0.07071068, 0.14142136, -111.084114]
        [0.0, 0.0, 1.0]}

     [0.0, -0.2, -0.120000005]
     [0.1, 0.0, 0.05]
     [0.0, 0.0, 1.0]}





 (1.000000, 0.000000, 0.000000, 0.000000)
   (0.000000, 1.000000, 0.000000, 0.000000)
   (0.000000, 0.000000, 1.000000, 0.000000)
   (0.500000, 0.600000, 0.000000, 1.000000)

     */

        val mMat = FloatArray(16)

        mMat[0] = array[Matrix.MSCALE_X]
        mMat[1] = 0f
        mMat[2] = 0f
        mMat[3] = 0f

        mMat[4] = 0.0f
        mMat[5] = array[Matrix.MSCALE_Y]
        mMat[6] = 0.0f
        mMat[7] = 0f

        mMat[8] = 0f
        mMat[9] = 0f
        mMat[10] = 1.0F
        mMat[11] = 0f

        mMat[12] = array[Matrix.MTRANS_X]
        mMat[13] = array[Matrix.MTRANS_Y]
        mMat[14] = 0f
        mMat[15] = 1f

        //((0.500000, 0.000000, 0.000000, 0.000000), (0.000000, 0.500000, 0.000000, 0.000000), (0.000000, 0.000000, 0.000000, 0.000000), (0.000000, 0.000000, 0.000000, 1.000000))
        //((0.500000, 0.000000, 0.000000, 0.000000), (0.000000, 0.500000, 0.000000, 0.000000), (0.000000, 0.000000, 1.000000, 0.000000), (0.500000, -0.500000, 0.000000, 1.000000))
        return mMat
    }


    fun getMatrix3fString(values: FloatArray): String {
        val sb = StringBuilder(64)
        sb.append("Matrix{")
        sb.append('[')
        sb.append(values[0])
        sb.append(", ")
        sb.append(values[1])
        sb.append(", ")
        sb.append(values[2])
        sb.append("][")
        sb.append(values[3])
        sb.append(", ")
        sb.append(values[4])
        sb.append(", ")
        sb.append(values[5])
        sb.append("][")
        sb.append(values[6])
        sb.append(", ")
        sb.append(values[7])
        sb.append(", ")
        sb.append(values[8])
        sb.append(']')
        sb.append("}")
        return sb.toString()
    }



}
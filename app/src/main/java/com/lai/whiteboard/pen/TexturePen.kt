package com.lai.whiteboard.pen

import android.graphics.PointF
import android.view.MotionEvent
import android.view.View
import android.view.ViewConfiguration
import com.lai.whiteboard.ShaderNative
import com.lai.whiteboard.pen.page.BrushInfoConfig
import com.lai.whiteboard.util.PointUtils
import kotlin.math.pow


/**
 *
 * @author  Lai
 *
 * @time 2021/1/12 1:36
 * @describe describe
 *
 */
class TexturePen : IPen {
    companion object {
        const val DRAW = 0
        const val ERASER = 1
    }

    private val mFromPoint = PointF()
    private val mToPoint = PointF()
    private val mControlPoint = PointF()
    private val mLastPoint = PointF()
    private val mCurrPoint = PointF()
    private val mDownPoint = PointF()

    private val mPoints = ArrayList<Float>()

    private var mDrawPoints = ArrayList<Float>()

    private var mCurrBrushConfig: BrushInfoConfig? = null

    override fun getDrawPoints(): ArrayList<Float> {
        return mDrawPoints
    }

    override fun drawPoints(points: FloatArray, textureRotate: Float) {
        ShaderNative.glDrawPaint(points, points.size / 2, textureRotate)
    }

    private var touchSlop = 0
    private var isMove = false

    override fun downEvent(pagerView: View, coordinate: PointF, event: MotionEvent) {
        if (touchSlop == 0) {
            touchSlop = ViewConfiguration.get(pagerView.context).scaledTouchSlop
        }
        mFromPoint.set(coordinate.x, coordinate.y)
        mDownPoint.set(coordinate.x, coordinate.y)
        mToPoint.set(coordinate.x, coordinate.y)
        mControlPoint.set(coordinate.x, coordinate.y)
        mLastPoint.set(coordinate.x, coordinate.y)
        isMove = false
    }


    override fun moveEvent(pagerView: View, coordinate: PointF, event: MotionEvent) {
        val distanceTo =
            PointUtils.getDistanceTo(mFromPoint.x, mFromPoint.y, coordinate.x, coordinate.y)

        if (distanceTo < touchSlop) return

        isMove = true

        mCurrPoint.set(coordinate.x, coordinate.y)

        // 起始点和当前的点重合，不需要绘制
        if (mFromPoint.equals(mCurrPoint.x, mControlPoint.y)) {
            return
        }
        mToPoint.set(PointUtils.multiplySum(mLastPoint, mCurrPoint, 0.5f))
        mControlPoint.set(mLastPoint)

        val list = PointUtils.pointsWithFrom(mFromPoint, mToPoint, mControlPoint,mCurrBrushConfig?.brushWidth)
        //list.removeAt(0)
        list.forEach {
            val points = PointUtils.vertexWithPoint(it.x, it.y, pagerView.width, pagerView.height)
            mPoints.add(points[0])
            mPoints.add(points[1])
        }
        if (mPoints.size == 0) {
            return
        }

        mDrawPoints.addAll(mPoints)
        //val randoms = (0..360).random()

        drawPoints(mPoints.toFloatArray(), 0.0f)

        mPoints.clear()

        mFromPoint.set(mToPoint)
        mLastPoint.set(coordinate.x, coordinate.y)

        /* val endX = (event.x + mControlPoint.x) / 2
         val endY = (event.y + mControlPoint.y) / 2

         mToPoint.set(endX, endY)

         PointUtils.pointsWithFrom(mFromPoint, mToPoint, mControlPoint).forEach {
             val points = PointUtils.vertexWithPoint(it.x, it.y, pagerView.width, pagerView.height)
             mPoints.add(points[0])
             mPoints.add(points[1])
         }
         mDrawPoints.addAll(mPoints)
         drawPoints(mPoints.toFloatArray())
         mPoints.clear()

         mFromPoint.set(mToPoint)
         mControlPoint.set(event.x, event.y)
 */
    }

    private fun smoothPoint(
        midPoint1: PointF,
        midPoint2: PointF,
        prev1: PointF,
        t: Float
    ): PointF {
        val a1 = ((1.0 - t.toDouble()).pow(2.0)).toFloat()
        val a2 = (2.0f * (1.0f - t) * t)
        val a3 = t * t

        return PointF(
            midPoint1.x * a1 + prev1.x * a2 + midPoint2.x * a3,
            midPoint1.y * a1 + prev1.y * a2 + midPoint2.y * a3
        )
    }

    override fun upEvent(pagerView: View, coordinate: PointF, event: MotionEvent) {
        //mPointIndex = 0
        if (!isMove) {
            val points = PointUtils.vertexWithPoint(
                coordinate.x,
                coordinate.y,
                pagerView.width,
                pagerView.height
            )
            mDrawPoints.add(points[0])
            mDrawPoints.add(points[1])
            drawPoints(mDrawPoints.toFloatArray(), 45f)
        }
    }

    override fun rest() {
        mDrawPoints.clear()
        mCurrBrushConfig = null
    }

    override fun copy(brushInfoConfig: BrushInfoConfig): IPen {
        val copy = TexturePen()
        copy.mCurrBrushConfig = brushInfoConfig.copy()
        copy.mCurrBrushConfig?.apply {
            points = mDrawPoints.toFloatArray()
            vertexCount = mDrawPoints.size / 2
        }

        return copy
    }

    override fun getConfig(): BrushInfoConfig? {
        return mCurrBrushConfig
    }
}
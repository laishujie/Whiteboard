package com.lai.whiteboard.pen

import android.graphics.PointF
import android.view.MotionEvent
import android.view.View
import android.view.ViewConfiguration
import com.lai.whiteboard.ShaderNative
import com.lai.whiteboard.util.PointUtils


/**
 *
 * @author  Lai
 *
 * @time 2021/1/12 1:36
 * @describe describe
 *
 */
class TexturePen : IPen {
    private val mFromPoint = PointF()
    private val mToPoint = PointF()
    private val mControlPoint = PointF()
    private val mPoints = ArrayList<Float>()
    private var mDrawPoints = ArrayList<Float>()

    override fun getDrawPoints(): ArrayList<Float> {
        return mDrawPoints
    }

    override fun drawPoints(points: FloatArray) {
        ShaderNative.glDrawPaint(points, points.size / 2)
    }

    private var touchSlop = 0
    private var isMove = false

    override fun downEvent(pagerView: View, event: MotionEvent) {
        if (touchSlop == 0) {
            touchSlop = ViewConfiguration.get(pagerView.context).scaledTouchSlop
        }
        mFromPoint.set(event.x, event.y)
        mToPoint.set(event.x, event.y)
        mControlPoint.set(event.x, event.y)
        isMove = false
    }


    override fun moveEvent(pagerView: View, event: MotionEvent) {

        val distanceTo = PointUtils.getDistanceTo(mFromPoint.x, mFromPoint.y, event.x, event.y)

        if (distanceTo < touchSlop) return

        isMove = true

        val endX = (event.x + mControlPoint.x) / 2
        val endY = (event.y + mControlPoint.y) / 2

        mToPoint.set(endX, endY)

        PointUtils.createBesselCurvePoints(mFromPoint, mToPoint, mControlPoint).forEach {
            val points = PointUtils.vertexWithPoint(it.x, it.y, pagerView.width, pagerView.height)
            mPoints.add(points[0])
            mPoints.add(points[1])
        }
        mDrawPoints.addAll(mPoints)
        drawPoints(mPoints.toFloatArray())
        mPoints.clear()

        mFromPoint.set(mToPoint)
        mControlPoint.set(event.x, event.y)

        /*
        线条不流畅
        mToPoint.set(event.x, event.y)
        mControlPoint.set(PointUtils.multiplySum(mToPoint,mFromPoint,0.5f))

        PointUtils.createBesselCurvePoints(mFromPoint, mToPoint, mControlPoint).forEach {
            val points = PointUtils.vertexWithPoint(it.x, it.y, pagerView.width, pagerView.height)
            mPoints.add(points[0])
            mPoints.add(points[1])
        }

        mDrawPoints.addAll(mPoints)
        drawPoints(mPoints.toFloatArray())
        mPoints.clear()

        mFromPoint.set(mToPoint)*/
    }

    override fun upEvent(pagerView: View, event: MotionEvent) {
        if (!isMove) {
            val points = PointUtils.vertexWithPoint(
                mToPoint.x,
                mToPoint.y,
                pagerView.width,
                pagerView.height
            )
            mDrawPoints.add(points[0])
            mDrawPoints.add(points[1])
            drawPoints(mDrawPoints.toFloatArray())
        }
    }

    override fun rest() {
        mDrawPoints.clear()
    }

    override fun copy(): IPen {
        val copy = TexturePen()
        copy.mDrawPoints = ArrayList(mDrawPoints)

        return copy
    }
}
package com.lai.whiteboard.widget

import android.content.Context
import android.graphics.Matrix
import android.graphics.PointF
import android.graphics.RectF
import android.util.AttributeSet
import android.util.Log
import android.view.MotionEvent
import android.view.View


/**
 *
 * @author  Lai
 *
 * @time 2021/1/27 3:22
 * @describe  触摸管理
 *
 */
class TouchView : View {

    //触摸类型
    enum class TouchType { PEN, WINDOW }

    //触摸事件接口回调
    var mTouchEvent: TouchEvent? = null

    //已经变换的Matrix用于坐标转换使用
    var mMatrix: Matrix = Matrix()
    var mInverseMatrix: Matrix = Matrix()

    //画板大小
    var mWhiteSizeRect = RectF()

    //真正点击所对应的点
    var readPointF = PointF()

    //是否需要重新下发down事件
    var isNeedAgainDown = false

    //是否需要绘制
    var isNeedDraw = true


    fun updateMatrixInfo(matrix: Matrix, mInit: RectF) {
        mMatrix.set(matrix)
        mWhiteSizeRect.set(mInit)
    }


    private fun transformCoordinate(event: MotionEvent?): FloatArray? {
        if (event == null) return null
        val dst = FloatArray(2)

        if (mMatrix.isIdentity) {
            dst[0] = event.x
            dst[1] = event.y
            return dst
        }
        mInverseMatrix.reset()
        mMatrix.invert(mInverseMatrix)
        mInverseMatrix.mapPoints(dst, floatArrayOf(event.x, event.y))
        if (mWhiteSizeRect.contains(dst[0], dst[1])) {
            return dst
        }
        return null
    }


    constructor(context: Context?) : super(context)
    constructor(context: Context?, attrs: AttributeSet?) : super(context, attrs)
    constructor(context: Context?, attrs: AttributeSet?, defStyleAttr: Int) : super(
        context,
        attrs,
        defStyleAttr
    )

    interface TouchEvent {
        fun down(type: TouchType, coordinate: PointF, event: MotionEvent, view: TouchView)
        fun move(type: TouchType, coordinate: PointF, event: MotionEvent, view: TouchView)
        fun up(type: TouchType, coordinate: PointF, event: MotionEvent, view: TouchView)
    }

    override fun onTouchEvent(event: MotionEvent?): Boolean {
        val transformCoordinate = transformCoordinate(event)

        transformCoordinate?.apply {
            readPointF.set(transformCoordinate[0], transformCoordinate[1])
        }

        when (event?.actionMasked) {
            MotionEvent.ACTION_DOWN -> {
                if (transformCoordinate != null && isNeedDraw) {
                    mTouchEvent?.down(TouchType.PEN, readPointF, event, this)
                }
            }
            MotionEvent.ACTION_POINTER_DOWN -> {
                //当第二个手指
                if (event.pointerCount == 2) {
                    isNeedDraw = false
                    mTouchEvent?.down(TouchType.WINDOW, readPointF, event, this)
                }
            }
            MotionEvent.ACTION_MOVE -> {
                if (event.pointerCount == 1 && isNeedDraw) {
                    //Log.e("11111", "pen MotionEvent.ACTION_MOVE")
                    if (transformCoordinate != null) {
                        if (isNeedAgainDown) {
                            mTouchEvent?.down(TouchType.PEN, readPointF, event, this)
                            isNeedAgainDown = false
                        }
                        mTouchEvent?.move(TouchType.PEN, readPointF, event, this)
                    } else {
                        isNeedAgainDown = true
                    }
                } else if (event.pointerCount == 2) {
                    mTouchEvent?.move(TouchType.WINDOW, readPointF, event, this)
                }
            }
            MotionEvent.ACTION_POINTER_UP -> {
                if (event.pointerCount == 2) {
                    mTouchEvent?.up(TouchType.WINDOW, readPointF, event, this)
                }
            }
            MotionEvent.ACTION_UP, MotionEvent.ACTION_CANCEL -> {
                //Log.e("11111", "pen MotionEvent.ACTION_MOVE")
                if (transformCoordinate != null && isNeedDraw)
                    mTouchEvent?.up(TouchType.PEN, readPointF, event, this)
                isNeedDraw = true
            }
        }
        return true
    }


}
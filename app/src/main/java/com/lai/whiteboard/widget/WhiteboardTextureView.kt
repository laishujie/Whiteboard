package com.lai.whiteboard.widget

import android.annotation.SuppressLint
import android.content.Context
import android.util.AttributeSet
import android.util.Log
import android.view.MotionEvent
import android.view.TextureView
import com.lai.whiteboard.ShaderNative
import com.lai.whiteboard.pen.IPen
import com.lai.whiteboard.pen.TexturePen
import java.util.*
import kotlin.collections.ArrayList

/**
 *
 * @author  Lai
 *
 * @time 2021/1/23 21:16
 * @describe describe
 *
 */
class WhiteboardTextureView : TextureView {

    private var currDrawPen = TexturePen()
    private var mDraPenArrayDeque = ArrayDeque<IPen>()
    private var mmBackArrayDeque = ArrayDeque<IPen>()
    private val mDrawPoints by lazy {
        ArrayList<Float>()
    }

    constructor(context: Context?) : this(context, null)
    constructor(context: Context?, attrs: AttributeSet?) : this(context, attrs, 0)
    constructor(context: Context?, attrs: AttributeSet?, defStyleAttr: Int) : super(
        context,
        attrs,
        defStyleAttr
    )


    @SuppressLint("ClickableViewAccessibility")
    override fun onTouchEvent(event: MotionEvent?): Boolean {
        when (event?.actionMasked) {
            MotionEvent.ACTION_DOWN -> {
                currDrawPen.downEvent(this, event)
            }
            MotionEvent.ACTION_MOVE -> {
                currDrawPen.moveEvent(this, event)
            }
            MotionEvent.ACTION_UP, MotionEvent.ACTION_CANCEL -> {
                currDrawPen.upEvent(this,event)
                if (currDrawPen.getDrawPoints().isNotEmpty()){
                    mDraPenArrayDeque.push(currDrawPen.copy())
                    currDrawPen.rest()
                }
            }
        }
        return true
    }


    //撤销
    fun undo() {
        if (mDraPenArrayDeque.isNotEmpty()) {
            ShaderNative.glClearPaintColor()
            mDrawPoints.clear()
            mmBackArrayDeque.push(mDraPenArrayDeque.pop())

            mDraPenArrayDeque.descendingIterator().forEach {
                mDrawPoints.addAll(it.getDrawPoints())
            }
            mmBackArrayDeque.last.drawPoints(mDrawPoints.toFloatArray())
        }
    }

    //返回
    fun redo() {
        if (mmBackArrayDeque.isNotEmpty()) {
            val pop1 = mmBackArrayDeque.pop()
            mDraPenArrayDeque.push(pop1)

            pop1.drawPoints(pop1.getDrawPoints().toFloatArray())
            /* if ([self.undoOperationStack isEmpty]) {
                 return;
             }
             MFPaintModel *model = self.undoOperationStack.topModel;
             [self.undoOperationStack popModel];
             [self.operationStack pushModel:model];

             [self drawModel:model];*/
            /* mDraPenArrayDeque.descendingIterator().forEach {
                 val drawPoints = it.getDrawPoints()
                 ShaderNative.glDrawPaint(drawPoints, drawPoints.size / 2)
             }*/
        }
    }

    fun glClearPaintColor() {
        mmBackArrayDeque.clear()
        mDraPenArrayDeque.clear()

        ShaderNative.glClearPaintColor()
    }

}

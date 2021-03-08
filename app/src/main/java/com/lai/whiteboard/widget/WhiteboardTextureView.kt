package com.lai.whiteboard.widget

import android.content.Context
import android.graphics.*
import android.util.AttributeSet
import android.util.Log
import android.view.MotionEvent
import android.view.Surface
import android.view.TextureView
import com.lai.whiteboard.ShaderNative
import com.lai.whiteboard.pen.IPen
import com.lai.whiteboard.pen.TexturePen
import com.lai.whiteboard.pen.page.BrushInfoConfig
import com.lai.whiteboard.pen.page.BrushManager
import com.lai.whiteboard.pen.page.WhiteboardActivity
import java.util.*
import kotlin.math.atan2
import kotlin.math.sqrt

/**
 *
 * @author  Lai
 *
 * @time 2021/1/23 21:16
 * @describe 画版TextureView
 *
 */
class WhiteboardTextureView : TextureView {
    //当前画笔
    private var mCurrDrawPen = TexturePen()

    //画了多少笔
    private var mDraPenArrayDeque = ArrayDeque<IPen>()

    //回退了多少笔
    private var mBackArrayDeque = ArrayDeque<IPen>()
    private val mLastMidPoint = PointF()

    //记录背景上一个点
    private val mBgLastPos = PointF()

    //背景矩阵
    private val mBgMatrix = Matrix()

    //TouchView 触摸事件
    private var mTouchView: TouchView? = null

    //触摸回调
    private var mTouchEvent: TouchView.TouchEvent? = null
    private val mLastVector = PointF()

    //状态回调
    private var mIWhiteboardStatus: IWhiteboardStatus? = null


    //记录上一次两只手指之间的距离
    private var mLastDist = 0f

    private val mBoundRectF = RectF()
    private val mInitRectF = RectF()

    private var mCurrBrushConfig: BrushInfoConfig? = null

    fun setBrushInfoConfig(currBrushConfig: BrushInfoConfig) {
        this.mCurrBrushConfig = currBrushConfig
    }

    interface IWhiteboardStatus {
        fun hasPen(undoDisable: Boolean, redoDisable: Boolean)
    }


    override fun onSizeChanged(w: Int, h: Int, oldw: Int, oldh: Int) {
        super.onSizeChanged(w, h, oldw, oldh)
        mInitRectF.set(0f, 0f, measuredWidth.toFloat(), measuredHeight.toFloat())
    }

    constructor(context: Context?) : this(context, null)
    constructor(context: Context?, attrs: AttributeSet?) : this(context, attrs, 0)
    constructor(context: Context?, attrs: AttributeSet?, defStyleAttr: Int) : super(
        context,
        attrs,
        defStyleAttr
    )

    fun init(touchView: TouchView, iStatus: IWhiteboardStatus, currBrushConfig: BrushInfoConfig) {
        registerTouchView(touchView)
        setBrushInfoConfig(currBrushConfig)
        mIWhiteboardStatus = iStatus
        surfaceTextureListener = object : SurfaceTextureListener {
            // SurfaceTexture 初始化好的时候调用
            override fun onSurfaceTextureAvailable(
                surface: SurfaceTexture?,
                width: Int,
                height: Int
            ) {
                if (surface == null) return

                ShaderNative.init(
                    width, height, Surface(surface),
                    BrushManager.getBrushBitmap(resources, currBrushConfig.res)
                ) {
                    Log.d(WhiteboardActivity.TAG, "INIT_CALLBACK")
                }
                //恢复数据
                drawAllData()
            }

            override fun onSurfaceTextureSizeChanged(
                surface: SurfaceTexture?,
                width: Int,
                height: Int
            ) {
            }

            override fun onSurfaceTextureUpdated(surface: SurfaceTexture?) {
            }

            override fun onSurfaceTextureDestroyed(surface: SurfaceTexture?): Boolean {
                ShaderNative.onDestroy()
                return true
            }
        }
    }


    private fun registerTouchView(touchView: TouchView) {
        mTouchView = touchView
        mTouchView?.updateMatrixInfo(mBgMatrix, mInitRectF)
        mTouchEvent = object : TouchView.TouchEvent {
            override fun down(
                type: TouchView.TouchType,
                coordinate: PointF,
                event: MotionEvent,
                view: TouchView
            ) {
                when (type) {
                    TouchView.TouchType.PEN -> {
                        mCurrDrawPen.downEvent(this@WhiteboardTextureView, coordinate, event)
                    }
                    TouchView.TouchType.WINDOW -> {
                        mBgLastPos.set(event.x, event.y)
                        //currDrawPen.rest()
                        //记录两手指的中点
                        val x = (event.getX(0) + event.getX(1)) / 2F
                        val y = (event.getY(0) + event.getY(1)) / 2F
                        //记录开始滑动前两手指中点的坐标
                        mLastMidPoint.set(x, y)
                        //记录开始滑动前两个手指之间的距离
                        mLastDist = distance(event)
                        //设置向量，以便于计算角度
                        mLastVector.set(
                            event.getX(0) - event.getX(1),
                            event.getY(0) - event.getY(1)
                        )
                    }
                }
            }

            override fun move(
                type: TouchView.TouchType,
                coordinate: PointF,
                event: MotionEvent,
                view: TouchView
            ) {
                when (type) {
                    TouchView.TouchType.PEN -> {
                        mCurrDrawPen.moveEvent(this@WhiteboardTextureView, coordinate, event)
                    }
                    TouchView.TouchType.WINDOW -> {
                        val dx: Float = event.x - mBgLastPos.x
                        val dy: Float = event.y - mBgLastPos.y
                        val scaleFactor = distance(event) / mLastDist
                        mLastMidPoint[mLastMidPoint.x + dx] = mLastMidPoint.y + dy
                        //当前两只手指构成的向量
                        val vector =
                            PointF(event.getX(0) - event.getX(1), event.getY(0) - event.getY(1))
                        //计算本次向量和上一次向量之间的夹角
                        //计算本次向量和上一次向量之间的夹角
                        val degree: Float = calculateDeltaDegree(mLastVector, vector)
                        setMatrix(dx, dy, scaleFactor, degree)
                        //更新mLastVector,以便下次旋转计算旋转过的角度
                        mLastVector.set(vector.x, vector.y)
                        mBgLastPos.set(event.x, event.y)
                        //重置mLastDist，让下次缩放在此基础上进行
                        mLastDist = distance(event)
                    }
                }
            }

            override fun up(
                type: TouchView.TouchType,
                coordinate: PointF,
                event: MotionEvent,
                view: TouchView
            ) {
                when (type) {
                    TouchView.TouchType.PEN -> {
                        mCurrDrawPen.upEvent(this@WhiteboardTextureView, coordinate, event)
                        if (mCurrDrawPen.getDrawPoints().isNotEmpty()) {
                            mCurrBrushConfig?.apply {
                                mDraPenArrayDeque.push(mCurrDrawPen.copy(this))
                                mIWhiteboardStatus?.hasPen(true, redoDisable = false)
                            }
                            mCurrDrawPen.rest()
                        }
                    }
                    TouchView.TouchType.WINDOW -> {
                        //重置旋转和缩放使用到的中点坐标
                        mLastMidPoint.set(0f, 0f)
                        //重置两只手指的距离
                        mLastDist = 0f
                    }
                }
                mTouchView?.updateMatrixInfo(mBgMatrix, mInitRectF)
            }
        }
        touchView.mTouchEvent = mTouchEvent
    }

    private fun setMatrix(dx: Float, dy: Float, sc: Float, r: Float) {
        upDateBoundRectF()
        mBgMatrix.postTranslate(dx, dy)
        mBgMatrix.postScale(sc, sc, mBoundRectF.centerX(), mBoundRectF.centerY())
        mBgMatrix.postRotate(r, mBoundRectF.centerX(), mBoundRectF.centerY())
        setTransform(mBgMatrix)
        invalidate()
    }


    private fun upDateBoundRectF() {
        if (mInitRectF.isEmpty) {
            mInitRectF.set(0f, 0f, width.toFloat(), height.toFloat())
        }
        mBoundRectF.set(0f, 0f, width.toFloat(), height.toFloat())
        mBgMatrix.mapRect(mBoundRectF)
    }


    //撤销
    fun undo() {
        if (mDraPenArrayDeque.isNotEmpty()) {

            mBackArrayDeque.push(mDraPenArrayDeque.pop())

            if (mDraPenArrayDeque.isEmpty()) {
                ShaderNative.glClearAll()
            } else {
                drawAllData()
            }
        }

        mIWhiteboardStatus?.hasPen(mDraPenArrayDeque.isNotEmpty(), mBackArrayDeque.isNotEmpty())
    }

    //返回
    fun redo() {
        if (mBackArrayDeque.isNotEmpty()) {
            val pop1 = mBackArrayDeque.pop()
            mDraPenArrayDeque.push(pop1)
            pop1.getConfig()?.let { config ->
                drawBrushInfoConfig(config, isClear = false, isDisplay = true)
            }
        }
        mIWhiteboardStatus?.hasPen(mDraPenArrayDeque.isNotEmpty(), mBackArrayDeque.isNotEmpty())
    }


    fun drawAllData() {
        if (mDraPenArrayDeque.isEmpty()) return
        var isClear = true
        var i = 0
        mDraPenArrayDeque.descendingIterator().forEach {

            it?.getConfig()?.let { config ->
                drawBrushInfoConfig(config, isClear, i == mDraPenArrayDeque.size - 1)
                i++
                isClear = false
            }
        }
    }


    private fun drawBrushInfoConfig(
        config: BrushInfoConfig,
        isClear: Boolean,
        isDisplay: Boolean
    ) {
        val a = Color.alpha(config.currColorInt) * 1f / 255f
        val r = Color.red(config.currColorInt) * 1f / 255f
        val g = Color.green(config.currColorInt) * 1f / 255f
        val b = Color.blue(config.currColorInt) * 1f / 255f
        val isRotate = config.res?.isRotate ?: true
        ShaderNative.glDrawData(
            config.points,
            config.vertexCount,
            BrushManager.getBrushBitmap(resources, config.res),
            config.brushWidth,
            config.outType, isRotate, a, r, g, b,
            isClear,
            isDisplay
        )
        if (isDisplay) {
            //恢复当前配置
            mCurrBrushConfig?.let { mCurrBrushConfig ->
                val a1 = Color.alpha(mCurrBrushConfig.currColorInt) * 1f / 255f
                val r1 = Color.red(mCurrBrushConfig.currColorInt) * 1f / 255f
                val g1 = Color.green(mCurrBrushConfig.currColorInt) * 1f / 255f
                val b1 = Color.blue(mCurrBrushConfig.currColorInt) * 1f / 255f
                val isRotate1 = mCurrBrushConfig.res?.isRotate ?: true
                ShaderNative.glDrawData(
                    null,
                    0,
                    BrushManager.getBrushBitmap(resources, mCurrBrushConfig.res),
                    mCurrBrushConfig.brushWidth,
                    mCurrBrushConfig.outType, isRotate1, a1, r1, g1, b1,
                    false,
                    isDisplay
                )
            }
        }
    }

    /**
     * 清理所有
     */
    fun glClearAll() {
        mBackArrayDeque.clear()
        mDraPenArrayDeque.clear()
        ShaderNative.glClearAll()

        mIWhiteboardStatus?.hasPen(undoDisable = false, redoDisable = false)
    }


    /**
     * 计算两个手指间的距离
     *
     * @param event 触摸事件
     * @return 放回两个手指之间的距离
     */
    private fun distance(event: MotionEvent): Float {
        val x = event.getX(0) - event.getX(1)
        val y = event.getY(0) - event.getY(1)
        return sqrt(x * x + y * y.toDouble()).toFloat() //两点间距离公式
    }

    /**
     * 计算两个向量之间的夹角
     *
     * @param lastVector 上一次两只手指形成的向量
     * @param vector     本次两只手指形成的向量
     * @return 返回手指旋转过的角度
     */
    private fun calculateDeltaDegree(lastVector: PointF, vector: PointF): Float {
        val lastDegree =
            atan2(lastVector.y.toDouble(), lastVector.x.toDouble()).toFloat()
        val degree =
            atan2(vector.y.toDouble(), vector.x.toDouble()).toFloat()
        val deltaDegree = degree - lastDegree
        return Math.toDegrees(deltaDegree.toDouble()).toFloat()
    }
}

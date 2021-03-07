package com.lai.whiteboard.pen.page

import android.content.DialogInterface
import android.graphics.Color
import android.os.Bundle
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import com.lai.whiteboard.R
import com.lai.whiteboard.ShaderNative
import com.lai.whiteboard.util.DialogUtils
import com.lai.whiteboard.util.PhotosHelper
import com.lai.whiteboard.util.StorageHelper
import com.lai.whiteboard.widget.ColorPickerView
import com.lai.whiteboard.widget.WhiteboardTextureView
import com.lai.whiteboard.widget.dialog.BrushDialogFragment
import com.lai.whiteboard.widget.dialog.ColorDialogFragment
import com.lai.whiteboard.widget.dialog.LoadingDialogFragment
import kotlinx.android.synthetic.main.activity_whiteboard.*
import java.io.File

/**
 *
 */
class WhiteboardActivity : AppCompatActivity() {
    //当前默认撤回返回res
    private var currUndoRes = R.drawable.ic_undo_no
    private var currRedoRes = R.drawable.ic_redo_no

    //默认配置
    private var currBrushConfig = BrushManager.getDefaultBrushInfoConfig()

    //画笔选择
    private val brushDialogFragment by lazy {
        var fragment =
            supportFragmentManager.findFragmentByTag("brushDialogFragment") as? BrushDialogFragment
        if (fragment == null) {
            fragment = BrushDialogFragment()
        }
        fragment!!
    }

    companion object {
        const val TAG = "Whiteboard"
    }


    private val loadingDialogFragment by lazy {
        var fragment =
            supportFragmentManager.findFragmentByTag("loadingDialogFragment") as? LoadingDialogFragment
        if (fragment == null) {
            fragment = LoadingDialogFragment()
        }
        fragment!!
    }


    private val colorDialogFragment by lazy {
        var fragment =
            supportFragmentManager.findFragmentByTag("colorDialogFragment") as? ColorDialogFragment
        if (fragment == null) {
            fragment = ColorDialogFragment()
        }
        fragment!!
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_whiteboard)

        white_view.init(touch_view, object : WhiteboardTextureView.IWhiteboardStatus {
            override fun hasPen(undoDisable: Boolean, redoDisable: Boolean) {
                val undoRes = if (undoDisable) {
                    R.drawable.ic_undo
                } else {
                    R.drawable.ic_undo_no
                }
                replaceUndoRes(undoRes)

                val redoRes = if (redoDisable) {
                    R.drawable.ic_redo
                } else {
                    R.drawable.ic_redo_no
                }
                replaceRedoRes(redoRes)
            }
        }, currBrushConfig)

        brushDialogFragment.select = object : BrushDialogFragment.IBrushSelect {
            override fun selectBrush(info: BrushInfoConfig) {

                val isRotate = info.res?.isRotate ?: true

                ShaderNative.glSetPaintTexture(
                    BrushManager.getBrushBitmap(
                        resources,
                        BrushManager.isAgainSetTexture(currBrushConfig.res?.id, info)
                    ),
                    isRotate,
                    info.brushWidth,
                    info.outType
                )

                currBrushConfig.res = info.res
                currBrushConfig.brushWidth = info.brushWidth
                currBrushConfig.outType = info.outType
            }
        }

        colorDialogFragment.mColorSelect = ColorPickerView.ColorSelect {
            currBrushConfig.currColorInt = it
            val a = Color.alpha(it) * 1f / 255f
            val r = Color.red(it) * 1f / 255f
            val g = Color.green(it) * 1f / 255f
            val b = Color.blue(it) * 1f / 255f
            ShaderNative.glPaintColor(a, r, g, b)
            colorDialogFragment.dismiss()
        }

        iv_pen.setOnClickListener {
            brushDialogFragment.show(supportFragmentManager, "brushDialogFragment")
        }

        iv_color.setOnClickListener {
            colorDialogFragment.restColor = currBrushConfig.currColorInt
            colorDialogFragment.show(supportFragmentManager, "colorDialogFragment")
        }

        iv_reset.setOnClickListener {
            DialogUtils.showTip(this, "是否清空内容？", DialogInterface.OnClickListener { d, _ ->
                white_view.glClearAll()
                d.dismiss()
            })
        }

        iv_redo.setOnClickListener {
            white_view.redo()
        }
        iv_undo.setOnClickListener {
            white_view.undo()
        }

        iv_save.setOnClickListener {
            val path =
                StorageHelper.getCameraPath(this) + File.separator + "${System.currentTimeMillis()}.png"

            DialogUtils.showTip(this, "是否保存？",
                DialogInterface.OnClickListener { dialog, _ ->
                    loadingDialogFragment.show(supportFragmentManager, "loadingDialogFragment")
                    ShaderNative.glSave(path) {
                        runOnUiThread {
                            loadingDialogFragment.dismiss()
                            val file = File(path)
                            PhotosHelper.refreshSystemPhoto(this, file)
                            Toast.makeText(this, "已保存到相册", Toast.LENGTH_SHORT).show()
                        }
                    }
                    dialog.dismiss()
                })

        }
    }


    fun replaceUndoRes(res: Int) {
        if (res == currUndoRes) return
        currUndoRes = res
        iv_undo.setImageResource(currUndoRes)
    }

    fun replaceRedoRes(res: Int) {
        if (res == currRedoRes) return
        currRedoRes = res
        iv_redo.setImageResource(currRedoRes)
    }

    override fun onBackPressed() {
        DialogUtils.showTip(this, "是否放弃？",
            DialogInterface.OnClickListener { dialog, _ ->
                dialog?.dismiss()
                super.onBackPressed()
            })
    }


}

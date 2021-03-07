package com.lai.whiteboard.widget.dialog

import android.os.Bundle
import android.view.View
import android.widget.RadioGroup
import android.widget.SeekBar
import android.widget.TextView
import androidx.recyclerview.widget.GridLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.lai.whiteboard.R
import com.lai.whiteboard.adapter.BrushAdapter
import com.lai.whiteboard.pen.TexturePen
import com.lai.whiteboard.pen.page.BrushInfoConfig
import com.lai.whiteboard.pen.page.BrushManager
import com.lai.whiteboard.pen.page.BrushRes


/**
 *
 * @author  Lai
 *
 * @time 2021/2/18 23:06
 * @describe describe
 *
 */


class BrushDialogFragment : BaseDialogFragment() {

    var rvList: RecyclerView? = null
    var select: IBrushSelect? = null
    var tvWidth: TextView? = null
    var mRbModel: RadioGroup? = null
    var sbWidth: SeekBar? = null

    private val selectBrushInfo by lazy {
        BrushManager.getDefaultBrushInfoConfig()
    }
    private val adapter = BrushAdapter()

    interface IBrushSelect {
        fun selectBrush(info: BrushInfoConfig)
    }

    override fun getLayoutId(): Int {
        return R.layout.dialog_brush
    }

    override fun initView(view: View, savedInstanceState: Bundle?) {
        rvList = view.findViewById(R.id.rv_list)

        if (rvList?.adapter == null) {
            rvList?.layoutManager = GridLayoutManager(view.context, 5)
            adapter.setOnItemClickListener { adapters, _, position ->
                selectBrushInfo.res = adapters.getItem(position) as BrushRes?
            }
            rvList?.adapter = adapter
            rvList?.itemAnimator = null
        }

        tvWidth = view.findViewById(R.id.tv_width)
        sbWidth = view.findViewById(R.id.sb_width)
        mRbModel = view.findViewById(R.id.rb_model)
        view.findViewById<TextView>(R.id.tv_select).setOnClickListener {
            select?.selectBrush(selectBrushInfo)
            dismiss()
        }

        sbWidth?.setOnSeekBarChangeListener(object :
            SeekBar.OnSeekBarChangeListener {
            override fun onProgressChanged(seekBar: SeekBar?, progress: Int, fromUser: Boolean) {
                tvWidth?.text = "宽度: $progress"
                selectBrushInfo.brushWidth = progress.toFloat()
            }

            override fun onStartTrackingTouch(seekBar: SeekBar?) {
            }

            override fun onStopTrackingTouch(seekBar: SeekBar?) {
            }
        })

        mRbModel?.setOnCheckedChangeListener { _, checkedId ->
            if (checkedId == R.id.rb_draw) {
                selectBrushInfo.outType = TexturePen.DRAW
            } else {
                selectBrushInfo.outType = TexturePen.ERASER
            }
        }
    }

}
package com.lai.whiteboard.widget.dialog

import android.graphics.Color
import android.os.Bundle
import android.view.View
import com.lai.whiteboard.R
import com.lai.whiteboard.widget.ColorPickerView
import com.lai.whiteboard.widget.ColorPickerView.ColorSelect

/**
 *
 * @author  Lai
 *
 * @time 2021/3/1 16:23
 * @describe describe
 *
 */
class ColorDialogFragment : BaseDialogFragment() {

    var colorPickerView: ColorPickerView? = null

     var mColorSelect: ColorSelect? = null

    var restColor = Color.BLACK

    override fun getLayoutId(): Int {
        return R.layout.dialog_color
    }

    override fun initView(view: View, savedInstanceState: Bundle?) {
        colorPickerView = view.findViewById(R.id.color_pick)
        colorPickerView?.restColor(restColor)
        colorPickerView?.colorSelect = mColorSelect
    }

    override fun isTouchClose(): Boolean {
        return true
    }

}
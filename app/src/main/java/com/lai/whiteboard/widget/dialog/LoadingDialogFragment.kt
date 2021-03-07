package com.lai.whiteboard.widget.dialog

import android.os.Bundle
import android.view.View
import com.lai.whiteboard.R

/**
 *
 * @author  Lai
 *
 * @time 2021/3/8 0:34
 * @describe describe
 *
 */
class LoadingDialogFragment : BaseDialogFragment() {
    override fun getLayoutId(): Int {
        return R.layout.dialog_loading
    }

    override fun initView(view: View, savedInstanceState: Bundle?) {
    }

    override fun isTouchClose(): Boolean {
        return false
    }

}
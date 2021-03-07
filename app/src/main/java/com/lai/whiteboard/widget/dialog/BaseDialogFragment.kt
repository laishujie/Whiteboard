package com.lai.whiteboard.widget.dialog

import android.graphics.Color
import android.graphics.drawable.ColorDrawable
import android.os.Bundle
import android.view.*
import androidx.annotation.LayoutRes
import androidx.appcompat.app.AppCompatDialogFragment

/**
 *
 * @author  Lai
 *
 * @time 2021/3/1 16:23
 * @describe describe
 *
 */
abstract class BaseDialogFragment : AppCompatDialogFragment() {

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        dialog?.apply {
            requestWindowFeature(Window.FEATURE_NO_TITLE)
            window?.setBackgroundDrawable(ColorDrawable(Color.TRANSPARENT))
            // setCancelable(false)
            //setCanceledOnTouchOutside(false)

            val dialogWindow = window!!
            val layoutParams = dialogWindow.attributes
            dialogWindow.decorView.setPadding(0, 0, 0, 0)
            layoutParams.gravity = Gravity.CENTER
            layoutParams.width = ViewGroup.LayoutParams.MATCH_PARENT
            layoutParams.height = ViewGroup.LayoutParams.WRAP_CONTENT
            dialogWindow.attributes = layoutParams

            setCancelable(isTouchClose())
            setCanceledOnTouchOutside(isTouchClose())
        }

        return inflater.inflate(getLayoutId(), container)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        initView(view, savedInstanceState)
    }

    @LayoutRes
    abstract fun getLayoutId(): Int

    abstract fun initView(view: View, savedInstanceState: Bundle?)

    protected open fun isTouchClose(): Boolean {
        return true
    }


}
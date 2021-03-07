package com.lai.whiteboard.util

import android.content.Context
import android.content.DialogInterface
import android.widget.Toast
import androidx.appcompat.app.AlertDialog
import com.lai.whiteboard.ShaderNative
import java.io.File

/**
 *
 * @author  Lai
 *
 * @time 2021/3/8 0:52
 * @describe describe
 *
 */
object DialogUtils {
    fun showTip(context: Context, content: String, listener: DialogInterface.OnClickListener) {
        AlertDialog.Builder(context).setTitle("提示").setMessage(content)
            .setPositiveButton("YES", listener).setNegativeButton("NO") { d, _ ->
                d.dismiss()
            }.show()
    }

}
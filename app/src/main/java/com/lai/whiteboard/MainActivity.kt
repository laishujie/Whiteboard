package com.lai.whiteboard

import android.content.Intent
import android.os.Bundle
import android.util.Log
import android.view.Surface
import android.view.SurfaceHolder
import androidx.appcompat.app.AppCompatActivity
import com.lai.whiteboard.pen.page.WhiteboardActivity
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        sample_text.setOnClickListener {
            startActivity(Intent(this, WhiteboardActivity::class.java))
        }
    }

}

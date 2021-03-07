package com.lai.whiteboard.adapter

import android.util.Log
import android.widget.ImageView
import com.chad.library.adapter.base.BaseQuickAdapter
import com.chad.library.adapter.base.listener.OnItemClickListener
import com.chad.library.adapter.base.viewholder.BaseViewHolder
import com.lai.whiteboard.R
import com.lai.whiteboard.pen.page.BrushManager
import com.lai.whiteboard.pen.page.BrushRes

/**
 * @author  Lai
 *
 * @time 2021/2/25 14:38
 * @describe describe
 */
class BrushAdapter : BaseQuickAdapter<BrushRes, BaseViewHolder>(
    R.layout.item_brush,
    BrushManager.getBrushList()
) {


    var lastSelectId = "1"
    var lastIndex = 0


    override fun convert(holder: BaseViewHolder, item: BrushRes) {
        if (lastSelectId == item.id) {
            holder.setBackgroundResource(R.id.iv_brush, R.drawable.brush_select)
        } else {
            holder.getView<ImageView>(R.id.iv_brush).background = null
        }
        holder.setImageResource(R.id.iv_brush, item.resBrush)
    }


    override fun setOnItemClickListener(listener: OnItemClickListener?) {
        super.setOnItemClickListener { adapter, view, position ->
            lastSelectId = data[position].id
            notifyItemChanged(lastIndex)
            notifyItemChanged(position)
            lastIndex = position
            listener?.onItemClick(adapter, view, position)
        }
    }

}
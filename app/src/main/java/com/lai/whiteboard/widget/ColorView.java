package com.lai.whiteboard.widget;

import android.content.Context;
import android.graphics.BitmapFactory;
import android.graphics.BitmapShader;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Paint.Style;
import android.graphics.Path;
import android.graphics.Path.Direction;
import android.graphics.RectF;
import android.graphics.Shader;
import android.util.AttributeSet;
import android.view.View;

import com.lai.whiteboard.R;

/**
 * This is a color preview View, that is used by ColorPickerView.
 * This class may be used as a standalone color preview also.
 */
public class ColorView extends View {
    private Paint mPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
    private BitmapShader mShader;
    private Path mClipPath = new Path();
    private RectF mRound = new RectF();

    private int mColor;

    {
        mShader = new BitmapShader(BitmapFactory.decodeResource(getContext().getResources(), R.drawable.checker), Shader.TileMode.REPEAT, Shader.TileMode.REPEAT);
    }

    public ColorView(Context context) {
        super(context);
    }

    public ColorView(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public ColorView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
    }

    @Override
    public void onDraw(Canvas canvas) {
        int width = getWidth();
        int height = getHeight();

        int radius = Math.min(width, height) / 2;
        int borderWidth = radius / 5;
        mClipPath.reset();
        mRound.set(borderWidth - 1, borderWidth - 1, width - borderWidth + 1, height - borderWidth + 1);
        mClipPath.addRoundRect(mRound, radius, radius, Direction.CW);

        canvas.save();
        canvas.clipPath(mClipPath);

        mPaint.setStyle(Style.FILL);
        mPaint.setShader(mShader);
        mPaint.setColor(0xffffffff);
        canvas.drawRect(0, 0, width, height, mPaint);

        canvas.restore();

        mPaint.setShader(null);
        mPaint.setColor(mColor);
        mRound.set(borderWidth - 2, borderWidth - 2, width - borderWidth + 2, height - borderWidth + 2);
        canvas.drawRoundRect(mRound, radius, radius, mPaint);

        mPaint.setStyle(Style.STROKE);
        mPaint.setStrokeWidth(borderWidth / 2f);
        mPaint.setColor(0xff000000);
        mRound.set(borderWidth / 4f, borderWidth / 4f, width - borderWidth / 4f, height - borderWidth / 4f);
        canvas.drawRoundRect(mRound, radius, radius, mPaint);
        mPaint.setColor(0xffffffff);
        mRound.set(borderWidth * 3f / 4 + 1, borderWidth * 3f / 4 + 1, width - borderWidth * 3f / 4 - 1, height - borderWidth * 3f / 4 - 1);
        canvas.drawRoundRect(mRound, radius, radius, mPaint);
    }

    public void setColor(int color) {
        mColor = color;
        invalidate();
    }

    public int getColor() {
        return mColor;
    }
}

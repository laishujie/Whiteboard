package com.lai.whiteboard.util;

import android.content.Context;
import android.content.Intent;
import android.net.Uri;

import java.io.File;

public class PhotosHelper {
    public static void refreshSystemPhoto(Context context, File file) {
        Intent mediaScanIntent = new Intent("android.intent.action.MEDIA_SCANNER_SCAN_FILE");
        mediaScanIntent.setData(Uri.fromFile(file));
        context.sendBroadcast(mediaScanIntent);
    }
}

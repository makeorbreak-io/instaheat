package com.chaveiro.instaheat.domain;

import android.content.Context;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;

public class AppData extends SQLiteOpenHelper {

    private static final int DATABASE_VERSION = 1;
    private static final String DATABASE_NAME = "APP_DB";
    private static final String TABLE_NAME = "SESSIONS";

    private AppData(Context context) {
        super(context, DATABASE_NAME, null, DATABASE_VERSION);
    }

    @Override
    public void onCreate(SQLiteDatabase db) {
        String createSessionsTable = "CREATE TABLE sessions ( " +
                "session_id INTEGER PRIMARY KEY AUTOINCREMENT, " +
                "temperature INTEGER, "+
                "time INTEGER )";
        db.execSQL(createSessionsTable);
    }

    @Override
    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
        db.execSQL("DROP TABLE IF EXISTS sessions");
        this.onCreate(db);
    }
}

package com.pong.game;

import org.libsdl.app.SDLActivity;

public class PongActivity extends SDLActivity {
    private static final String TAG = "PongActivity";
    
    //return the shared library name provided in the top CMakeLists.txt
    @Override
    protected String[] getLibraries(){
        return new String[] {
            "PongGame"
        };
    }
}

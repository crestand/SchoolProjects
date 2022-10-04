package BreakTheBlocks;

import javafx.scene.media.*;
import javafx.util.Duration;

import java.io.File;

/**
 * A class that handles the sound I/O
 */
public class SoundHandler {

    String blockBreaking = "src/resources/blockBreaking.wav";
    String backGround = "src/resources/BackGround.mp3";

    Media blockBreakingMedia = new Media(new File(blockBreaking).toURI().toString());
    MediaPlayer mediaPlayer = null;


    /**
     * Plays the bg music on loop
     */
    public void playBackgroundMusic() {
        MediaPlayer backGroundMediaPlayer = new MediaPlayer(new Media(new File(backGround).toURI().toString()));
        backGroundMediaPlayer.setOnEndOfMedia(() -> backGroundMediaPlayer.seek(Duration.ZERO));
        backGroundMediaPlayer.setVolume(0.05);
        backGroundMediaPlayer.play();
    }

    /**
     * Plays a block breaking sound
     */
    public void playDamageSound() {
        mediaPlayer = new MediaPlayer(blockBreakingMedia);
        mediaPlayer.play();
    }

}

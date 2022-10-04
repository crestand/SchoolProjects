package BreakTheBlocks;

import java.util.ArrayList;

/**
 * A class that implements high score function
 */
public class HighScoresHandler {

    FileHandler fh = new FileHandler();
    ArrayList<String[]> highScore;


    public HighScoresHandler() {
        highScore = fh.readHighScore();
        orderScores();
    }

    /**
     * Saves the Player's score
     * @param playerName Player's name
     * @param score Player's score
     */
    public void saveGame(String playerName, int score) {
        fh.writeToFile(playerName, score);
    }

    public ArrayList<String[]> getHighScore() {
        return highScore;
    }


    /**
     * Orders the highScore array in descending order
     */
    private void orderScores() {
        for (int i = 0; i < highScore.size() - 1; i++) {
            for (int j = 0; j < highScore.size() - i - 1; j++) {
                if (Integer.parseInt(highScore.get(j)[1]) < Integer.parseInt(highScore.get(j + 1)[1])) {
                    String[] tempString = {highScore.get(j)[0], highScore.get(j)[1]};
                    highScore.set(j, highScore.get(j + 1));
                    highScore.set(j + 1, tempString);
                }
            }
        }
    }

}

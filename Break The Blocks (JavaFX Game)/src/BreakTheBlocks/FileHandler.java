package BreakTheBlocks;

import java.io.File;  // Import the File class
import java.io.FileNotFoundException;  // Import this class to handle errors
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Locale;
import java.util.Scanner; // Import the Scanner class to read text files

/**
 * A class that handles file I/O
 */
public class FileHandler {


    private int currentLevel = 1;

    /**
     * Reads the next level file and populates a 2D Array with corresponding info.
     * Each row represents a line and in each line indexes represents:
     * [0] -> blockType , [1] -> row , [2] -> column
     *
     * @return ArrayList - An array that contains next level info.
     */
    public ArrayList<String[]> readNextLevel() {
        ArrayList<String[]> levelInfo = new ArrayList<>();

        String levelName = "levels/level" + currentLevel + ".txt";
        try {
            File myObj = new File(levelName);
            Scanner myReader = new Scanner(myObj);

            while (myReader.hasNextLine()) {
                String data = myReader.nextLine();
                levelInfo.add(data.split(","));
            }
            myReader.close();
        } catch (FileNotFoundException e) {
            levelInfo.clear();
            String[] temp = new String[]{
                    "-1"
            };
            levelInfo.add(temp);

        }
        currentLevel++;
        return levelInfo;
    }

    /**
     * Reads the high scores file and populates a 2D Array with corresponding info.
     * Each row represents a line and in each line indexes represents:
     * [0] -> Player's name , [1] -> Player's score
     *
     * @return ArrayList - An array that contains high scores info.
     */
    public ArrayList<String[]> readHighScore() {
        ArrayList<String[]> highScores = new ArrayList<>();

        try {
            File myObj = new File("highscores.dat");
            Scanner myReader = new Scanner(myObj);

            while (myReader.hasNextLine()) {
                String data = myReader.nextLine();
                highScores.add(data.split(","));
            }
            myReader.close();
        } catch (FileNotFoundException ignored) {

        }
        return highScores;
    }


    /**
     * Saves the given player's name and score
     * @param playerName
     * @param score
     */
    public void writeToFile(String playerName, int score) {
        File file = new File("highscores.dat");
        try {
            file.createNewFile();
            FileWriter fileWriter = new FileWriter("highscores.dat",true);

            fileWriter.append(playerName.toUpperCase(Locale.ROOT)).append(",").append(String.valueOf(score)).append("\n");
            fileWriter.close();
        } catch (IOException e) {
            System.out.println("Couldn't create the save file!!");
        }


    }

    public int getCurrentLevel() {
        return currentLevel;
    }


}

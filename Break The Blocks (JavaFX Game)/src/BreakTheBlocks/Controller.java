package BreakTheBlocks;

import javafx.application.Application;
import javafx.scene.text.Font;
import javafx.stage.Stage;
import java.io.FileInputStream;
import java.io.FileNotFoundException;

/**
 * A root class that contains and manages all scenes
 */
public class Controller extends Application {

    static Font font40;
    static Font font35;
    static Font font75;

    static {
        try {
            font40 = Font.loadFont(new FileInputStream("src/resources/fonts/BabyParty.ttf"), 40);
            font35 = Font.loadFont(new FileInputStream("src/resources/fonts/BabyParty.ttf"), 35);
            font75 = Font.loadFont(new FileInputStream("src/resources/fonts/BabyParty.ttf"), 75);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
    }

    static final int HEIGHT = 900;
    static final int WIDTH = 720;

    MainMenu mainMenu;
    Stage primaryStage;
    GameScene gameScene;
    EndGameScene endGameScene;
    SoundHandler sh;

    @Override
    public void start(Stage primaryStage) {
        this.primaryStage = primaryStage;
        primaryStage.setTitle("Break the Blocks");
        sh = new SoundHandler();
        sh.playBackgroundMusic();


        initializeMenu();
        primaryStage.setResizable(false);
        primaryStage.show();
    }

    /**
     * Initializes the Main Menu
     */
    public void initializeMenu() {
        mainMenu = new MainMenu(primaryStage);
        mainMenu.getStartButton().setOnMouseClicked(mouseEvent -> {
            try {
                startGame();
            } catch (FileNotFoundException e) {
                System.out.println("Couldn't find any level.");;
            }
        });

        mainMenu.getHighScoresButton().setOnMouseClicked(mouseEvent -> {
            HighScoresScene highScoresScene = new HighScoresScene(this);
            primaryStage.setScene(highScoresScene.getHighScoreScene());
        });

        primaryStage.setScene(mainMenu.getMainScene());
    }

    /**
     * Initializes and loads the Game Scene
     * @throws FileNotFoundException There is no level
     */
    private void startGame() throws FileNotFoundException {
        gameScene = new GameScene(primaryStage,this);
        gameScene.generateLevel();
        primaryStage.setScene(gameScene.getGameScene());
    }

    /**
     * Initializes and loads the End Game Scene
     * @param gameScore
     */
    public void endGame(int gameScore) {
        endGameScene = new EndGameScene(this, gameScore);
        primaryStage.setScene(endGameScene.getEndGameScene());
    }


    public Stage getPrimaryStage() {
        return primaryStage;
    }
    public static void main(String[] args) {
        launch(args);
    }
}

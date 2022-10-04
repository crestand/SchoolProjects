package BreakTheBlocks;

import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.layout.AnchorPane;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.Pane;
import javafx.scene.paint.Color;
import javafx.scene.text.Font;
import javafx.scene.text.Text;

import java.util.ArrayList;
import java.util.Locale;

/**
 * A class that manages high scores scene
 */
public class HighScoresScene {

    private AnchorPane highScorePane;
    private Scene highScoreScene;
    private Controller controller;
    private HighScoresHandler highScoresHandler;
    ArrayList<String[]> highScore;
    Font font;

    CustomButton mainMenuButton;
    GridPane scores;

    public HighScoresScene(Controller controller) {
        font = Controller.font40;

        this.controller = controller;
        highScoresHandler = new HighScoresHandler();
        highScorePane = new AnchorPane();
        highScoreScene = new Scene(highScorePane, Controller.WIDTH, Controller.HEIGHT);
        highScorePane.setStyle("-fx-background-image: url('/BackgroundYellow.png'); " +
                "-fx-background-position: center; " +
                "-fx-background-size: 100%;");

        highScore = highScoresHandler.getHighScore();
        initializeGUI();


    }

    /**
     * Initializes all GUI elements
     */
    private void initializeGUI() {

        Pane highScoresPanel = new Pane();
        highScoresPanel.setStyle("-fx-background-image: url('HighScoresPanel.png')");
        highScorePane.getChildren().add(highScoresPanel);
        highScoresPanel.setMinSize(680,690);
        highScoresPanel.setMaxSize(680,690);
        highScoresPanel.setLayoutX(15);
        highScoresPanel.setLayoutY(15);

        scores = new GridPane();
        scores.setAlignment(Pos.CENTER);
        scores.setHgap(20);
        scores.setVgap(5);
        highScoresPanel.getChildren().add(scores);

        // Prints top 5 Player's name
        try {
            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 2; j++) {
                    Text temp = new Text(highScore.get(i)[j].toUpperCase(Locale.ROOT));
                    temp.setFont(Controller.font75);
                    temp.setFill(Color.WHITE);
                    scores.add(temp, j, i);
                }
            }
        } catch (IndexOutOfBoundsException ignore) {

        }
        scores.setMinSize(680, 690);
        scores.setLayoutX(4);
        scores.setLayoutY(25);



        mainMenuButton = new CustomButton("MAIN MENU");
        highScorePane.getChildren().add(mainMenuButton);
        mainMenuButton.setLayoutX(360-173.5);
        mainMenuButton.setLayoutY(720);
        mainMenuButton.setOnMouseClicked(mouseEvent -> controller.initializeMenu());

    }

    public Scene getHighScoreScene() {
        return highScoreScene;
    }
}

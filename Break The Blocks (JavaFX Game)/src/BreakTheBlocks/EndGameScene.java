package BreakTheBlocks;

import javafx.scene.Scene;
import javafx.scene.control.TextField;
import javafx.scene.control.TextFormatter;
import javafx.scene.layout.AnchorPane;
import javafx.scene.layout.Pane;
import javafx.scene.paint.Color;
import javafx.scene.text.Text;
import javafx.scene.text.TextAlignment;
import javafx.stage.Stage;

/**
 * A class that manages the end game scene
 */
public class EndGameScene {

    private Stage primaryStage;
    private AnchorPane endGamePane;
    private Scene endGameScene;
    private Controller controller;
    private HighScoresHandler scoresHandler;
    private TextField playerName;

    int finalScore = 0;
    CustomButton mainMenuButton;



    public EndGameScene(Controller controller, int finalScore) {
        this.controller = controller;
        this.finalScore = finalScore;
        this.primaryStage = controller.getPrimaryStage();
        endGamePane = new AnchorPane();
        endGameScene = new Scene(endGamePane, Controller.WIDTH, Controller.HEIGHT);
        endGamePane.setStyle("-fx-background-image: url('/BackgroundYellow.png'); " +
                "-fx-background-position: center; " +
                "-fx-background-size: 100%;");

        initializeGUI();
        scoresHandler = new HighScoresHandler();

    }

    /**
     * Initializes all GUI elements
     */
    private void initializeGUI() {

        Pane finalScorePanel = new Pane();
        finalScorePanel.setStyle("-fx-background-image: url('/FinalScorePanel.png')");
        finalScorePanel.setMinSize(500, 205);
        endGamePane.getChildren().add(finalScorePanel);
        finalScorePanel.setLayoutX(110);
        finalScorePanel.setLayoutY(30);

        Text finalScoreText = new Text(String.valueOf(finalScore));
        finalScoreText.setFont(Controller.font75);
        finalScoreText.setFill(Color.WHITE);
        finalScoreText.setTextAlignment(TextAlignment.CENTER);
        finalScorePanel.getChildren().add(finalScoreText);
        finalScoreText.setLayoutX(220);
        finalScoreText.setLayoutY(175);

        int maxLength = 9;
        playerName = new TextField();
        endGamePane.getChildren().add(playerName);
        playerName.setStyle("-fx-background-color:transparent; -fx-background-image: url('/TextFieldBackground.png'); -fx-text-inner-color: white;");
        playerName.setTextFormatter(new TextFormatter<>((change) -> {

            //Limits the player's name to maxLength
            if (change.getControlNewText().length() > maxLength) {
                change.setText("");
                return change;
            }
            change.setText(change.getText().toUpperCase());
            return change;
        }));
        playerName.setFont(Controller.font35);
        playerName.setMinSize(445,70);
        playerName.setMaxSize(445,70);
        playerName.setLayoutY(250);
        playerName.setLayoutX(140);

        CustomButton saveButton = new CustomButton("SAVE");
        endGamePane.getChildren().add(saveButton);
        saveButton.setLayoutY(340);
        saveButton.setLayoutX(186.5);
        saveButton.setOnMouseClicked(mouseEvent -> {
            scoresHandler.saveGame(playerName.getText(),finalScore);
            saveButton.setDisable(true);
        });

        mainMenuButton = new CustomButton("MAIN MENU");
        endGamePane.getChildren().add(mainMenuButton);
        mainMenuButton.setLayoutY(520);
        mainMenuButton.setLayoutX(186.5);
        mainMenuButton.setOnMouseClicked(mouseEvent -> controller.initializeMenu());


        CustomButton exitButton = new CustomButton("EXIT");
        endGamePane.getChildren().add(exitButton);
        exitButton.setLayoutY(700);
        exitButton.setLayoutX(186.5);
        exitButton.setOnMouseClicked(mouseEvent -> primaryStage.close());
    }

    public Scene getEndGameScene() {
        return endGameScene;
    }
}

package BreakTheBlocks;

import javafx.scene.Scene;
import javafx.scene.layout.AnchorPane;
import javafx.scene.layout.Pane;
import javafx.stage.Stage;

/**
 * A class that manages Main Menu
 */
public class MainMenu {

    private Stage primaryStage;
    private AnchorPane mainPane;
    private Scene mainScene;
    CustomButton startButton;
    CustomButton highScoresButton;

    public MainMenu(Stage primaryStage) {
        this.primaryStage = primaryStage;
        mainPane = new AnchorPane();
        mainScene = new Scene(mainPane, Controller.WIDTH, Controller.HEIGHT);
        mainPane.setStyle("-fx-background-image: url('/BackgroundYellow.png'); " +
                "-fx-background-position: center; " +
                "-fx-background-size: 100%;");

        createButtons();
        createLogo();
    }

    /**
     * Initializes Buttons
     */
    private void createButtons() {
        startButton = new CustomButton("START");
        mainPane.getChildren().add(startButton);
        startButton.setLayoutY(300);
        startButton.setLayoutX(186.5);

        highScoresButton = new CustomButton("HIGH SCORES");
        mainPane.getChildren().add(highScoresButton);
        highScoresButton.setLayoutY(500);
        highScoresButton.setLayoutX(186.5);

        CustomButton exitButton = new CustomButton("EXIT");
        mainPane.getChildren().add(exitButton);
        exitButton.setLayoutY(700);
        exitButton.setLayoutX(186.5);
        exitButton.setOnMouseClicked(mouseEvent -> primaryStage.close());
    }

    /**
     * Initializes Logo
     */
    private void createLogo() {
        Pane logo = new Pane();
        logo.setMinSize(720,300);
        logo.setStyle("-fx-background-image: url('/Logo.png'); " +
                "-fx-background-position: center; " +
                "-fx-background-size: 100%;");
        mainPane.getChildren().add(logo);

    }

    public CustomButton getStartButton() {
        return startButton;
    }

    public CustomButton getHighScoresButton() {
        return highScoresButton;
    }

    public Scene getMainScene() {
        return mainScene;
    }

}

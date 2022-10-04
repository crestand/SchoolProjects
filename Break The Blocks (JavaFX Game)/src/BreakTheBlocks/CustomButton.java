package BreakTheBlocks;

import javafx.scene.control.Button;
import javafx.scene.input.MouseButton;
import javafx.scene.paint.Color;
import javafx.scene.paint.Paint;
import javafx.scene.text.TextAlignment;

/**
 * A class that implements custom button
 */
public class CustomButton extends Button {

    private final String BUTTON_PRESSED_STYLE = "-fx-background-color:transparent; -fx-background-image: url('/YellowButtonPressed.png');";
    private final String BUTTON_NORMAL_STYLE = "-fx-background-color:transparent; -fx-background-image: url('/YellowButton.png');";


    public CustomButton(String text) {
        setText(text);
        setTextFill(Color.WHITE);
        setTextAlignment(TextAlignment.CENTER);
        setButtonFont();
        setPrefWidth(347);
        setPrefHeight(164);
        initializeButtonListeners();
        setBUTTON_NORMAL_STYLE();
    }


    private void setButtonFont() {
        setFont(Controller.font40);
    }


    private void setBUTTON_PRESSED_STYLE() {
        setStyle(BUTTON_PRESSED_STYLE);
        setPrefHeight(159);
        setLayoutY(getLayoutY() + 5);
    }

    private void setBUTTON_NORMAL_STYLE() {
        setStyle(BUTTON_NORMAL_STYLE);
        setPrefHeight(164);
        setLayoutY(getLayoutY() - 5);
    }

    private void initializeButtonListeners() {
        setOnMousePressed(mouseEvent -> {
            if (mouseEvent.getButton().equals(MouseButton.PRIMARY)) {
                setBUTTON_PRESSED_STYLE();
            }
        });

        setOnMouseReleased(mouseEvent -> {
            if (mouseEvent.getButton().equals(MouseButton.PRIMARY)) {
                setBUTTON_NORMAL_STYLE();
            }
        });
    }


}

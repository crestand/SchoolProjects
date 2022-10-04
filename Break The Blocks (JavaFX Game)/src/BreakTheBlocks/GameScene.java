package BreakTheBlocks;

import javafx.geometry.Pos;
import javafx.scene.Node;
import javafx.scene.Scene;
import javafx.scene.layout.AnchorPane;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.Pane;
import javafx.scene.paint.Color;
import javafx.scene.text.Font;
import javafx.scene.text.Text;
import javafx.scene.text.TextAlignment;
import javafx.stage.Stage;

import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.List;

/**
 * A class that contains and manages all the things
 * about the Game Scene
 */
public class GameScene {

    public static final int ROW = 10;
    public static final int COL = 10;

    private Stage primaryStage;
    private AnchorPane gamePane;
    private Scene gameScene;
    private Controller controller;
    Font font;

    GameLogic gameLogic;
    int currentScore = 0;
    List<Block> clickedBlocks = new ArrayList<>();
    Block[][] board;
    GridPane playableGridArea;

    Text currentScoreText;
    Text turnScoreText;
    Text currentLevelText;
    Text hitBlockText;


    public GameScene(Stage primaryStage, Controller controller) {
        font = Controller.font40;

        this.controller = controller;
        this.primaryStage = primaryStage;
        gamePane = new AnchorPane();
        gameScene = new Scene(gamePane, Controller.WIDTH, Controller.HEIGHT);
        gamePane.setStyle("-fx-background-image: url('/BackgroundYellow.png'); " +
                "-fx-background-position: center; " +
                "-fx-background-size: 100%;");

        board = new Block[ROW][COL];
        gameLogic = new GameLogic(board);

        initializePlayableGridArea();
        initializeGUI();


    }

    /**
     * Initializes all GUI elements
     */
    private void initializeGUI() {


        Pane topContainer = new Pane();
        topContainer.setStyle("-fx-background-image: url('/TopContainerYellow.png')");
        topContainer.setMinSize(720, 192);
        topContainer.setMaxSize(720, 192);
        gamePane.getChildren().add(topContainer);
        topContainer.setLayoutY(-5);


        Pane scoreHolder = new Pane();
        scoreHolder.setStyle("-fx-background-image: url('/ScoreHolder.png')");
        scoreHolder.setMinSize(326, 102);
        topContainer.getChildren().add(scoreHolder);
        scoreHolder.setLayoutX(200);
        scoreHolder.setLayoutY(40);

        currentScoreText = new Text("0");
        currentScoreText.setFont(Controller.font40);
        currentScoreText.setFill(Color.WHITE);
        scoreHolder.getChildren().add(currentScoreText);
        currentScoreText.setLayoutY(61);
        currentScoreText.setLayoutX(235);

        turnScoreText = new Text("");
        turnScoreText.setFont(Controller.font40);
        turnScoreText.setFill(Color.web("#ffdc16"));
        scoreHolder.getChildren().add(turnScoreText);
        turnScoreText.setLayoutY(61);
        turnScoreText.setLayoutX(27);


        Pane levelRibbon = new Pane();
        levelRibbon.setStyle("-fx-background-image: url('/LevelRibbon.png');");
        levelRibbon.setMinSize(130, 180);
        levelRibbon.setMaxSize(130, 180);
        topContainer.getChildren().add(levelRibbon);
        levelRibbon.setLayoutX(30);
        levelRibbon.setLayoutY(0);

        currentLevelText = new Text("Level\n" + gameLogic.fh.getCurrentLevel());
        currentLevelText.setFont(Controller.font40);
        currentLevelText.setFill(Color.WHITE);
        currentLevelText.setTextAlignment(TextAlignment.CENTER);
        levelRibbon.getChildren().add(currentLevelText);
        currentLevelText.setLayoutY(60);
        currentLevelText.setLayoutX(7);


        Pane bottomContainer = new Pane();
        bottomContainer.setStyle("-fx-background-image: url('/BottomContainer.png');");
        bottomContainer.setMinSize(720, 100);
        bottomContainer.setMaxSize(720, 100);
        gamePane.getChildren().add(bottomContainer);
        bottomContainer.setLayoutY(820);

        hitBlockText = new Text("");
        hitBlockText.setFont(Controller.font35);
        hitBlockText.setFill(Color.WHITE);
        hitBlockText.setTextAlignment(TextAlignment.CENTER);
        bottomContainer.getChildren().add(hitBlockText);
        hitBlockText.setLayoutY(50);
        hitBlockText.setLayoutX(12);

    }

    /**
     * Initializes the playing grid
     */
    private void initializePlayableGridArea() {
        playableGridArea = new GridPane();
        playableGridArea.setMinSize(720, 720);
        playableGridArea.setVgap(1);
        playableGridArea.setHgap(1);
        playableGridArea.setAlignment(Pos.CENTER);
        playableGridArea.setStyle("-fx-background-color: transparent;");
        gamePane.getChildren().add(playableGridArea);
        playableGridArea.setLayoutY(150);
    }

    /**
     * Generates level by first getting next level info
     * If there isn't any level left it starts end game sequence,
     * otherwise it populates playing grid
     */
    public void generateLevel() {

        if (gameLogic.getLevelInfo()) {
            for (int row = 0; row < board.length; row++) {
                for (int col = 0; col < board[0].length; col++) {

                    //System.out.println(gameLogic.checkSlot(row, col));
                    BlockType slotType = gameLogic.checkSlot(row, col);
                    board[row][col] = new Block(row, col, slotType);

                    // Adds the mouse click handler to the blocks
                    if (board[row][col].getType() != BlockType.WALL) {
                        board[row][col].setOnMouseClicked(mouseEvent -> {
                            int col1, row1;
                            // After getting the target of the mouse click - in our case its the Block object-
                            // we parse it as a node and tell to GridPane to return the indexes of these Nodes
                            col1 = GridPane.getColumnIndex((Node) mouseEvent.getTarget());
                            row1 = GridPane.getRowIndex((Node) mouseEvent.getTarget());
                            clickedBlocks = gameLogic.onMouseClick(row1, col1);
                            currentScore = gameLogic.getCurrentScore();
                            try {
                                updateGUI();
                            } catch (FileNotFoundException e) {
                                e.printStackTrace();
                            }
                        });
                    }
                    playableGridArea.add(board[row][col], col, row);
                }
            }
        } else {
            initializeEndGameSequence();
        }


    }


    /**
     * Updates GUI after every click.
     * If there isn't any Block's left load the next level
     * @throws FileNotFoundException
     */
    private void updateGUI() throws FileNotFoundException {
        currentScoreText.setText(String.valueOf(currentScore));
        hitBlockText.setText(hitBlocksAppender());
        turnScoreText.setText(turnPointModifier(gameLogic.calculatePoint()));

        if (Block.getBlocksLeft() == 0) {
            currentLevelText.setText("Level\n" + gameLogic.fh.getCurrentLevel());
            generateLevel();
        }
    }

    /**
     * Returns the final string about the clicked Block's
     * coordinate information
     * @return String All clicked Block's x,y information
     */
    private String hitBlocksAppender() {
        StringBuilder appendedString = new StringBuilder();
        for (Block block : clickedBlocks) {
            appendedString.append("Hit: ").append(block.getRow()).append(",").append(block.getColumn()).append(" ");
        }
        return appendedString.toString();
    }

    /**
     * Takes the turn point and modifies it for GUI
     * @param turnPoint int - Amount of point that Player earned that turn
     * @return String Modified turnPoint
     */
    private String turnPointModifier(int turnPoint) {
        if (turnPoint > 0) {
            return "+" + turnPoint;
        } else if (turnPoint == 0) {
            return "--";
        } else {
            return String.valueOf(turnPoint);
        }
    }

    public void initializeEndGameSequence() {
        controller.endGame(currentScore);
    }

    public Scene getGameScene() {
        return gameScene;
    }
}

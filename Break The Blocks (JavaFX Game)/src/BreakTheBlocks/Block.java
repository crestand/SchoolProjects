package BreakTheBlocks;

import javafx.scene.layout.Pane;
import java.io.FileNotFoundException;

/**
 * A class
 */
public class Block extends Pane {

    private final int row;
    private final int column;
    private BlockType type;
    private int health;
    private static int blocksLeft = 0;


    private final String PURPLE_STYLE = "-fx-background-color:transparent; -fx-background-image: url('/SquarePurple.png');";
    private final String BLUE_STYLE = "-fx-background-color:transparent; -fx-background-image: url('/SquareBlue.png');";
    private final String WALL_STYLE = "-fx-background-color:transparent; -fx-background-image: url('/SquareWall.png');";
    private final String EMPTY_STYLE = "-fx-background-color:transparent; -fx-background-image: url('/SquareEmpty.png');";




    /**
     * @param row Row index of this Block
     * @param column Column index of this Block
     * @param type Type of this Block
     * @throws FileNotFoundException
     * @see BlockType
     */
    public Block(int row, int column, BlockType type) {
        this.row = row;
        this.column = column;
        this.type = type;
        setMinSize(60,60);
        setMaxSize(60,60);


        switch (this.type) {
            case WALL -> {
                health = 0;
                setStyle(WALL_STYLE);
            }
            case EMPTY -> {
                health = 0;
                setStyle(EMPTY_STYLE);
            }
            case BLUE -> {
                health = 1;
                blocksLeft++;
                setStyle(BLUE_STYLE);
            }
            case PURPLE -> {
                health = 2;
                blocksLeft++;
                setStyle(PURPLE_STYLE);
            }
        }
    }

    /**
     * Returns the row of the Block
     * @return int - row index
     */
    public int getRow() {
        return row;
    }

    /**
     * Returns the column of the block
     * @return int - column index
     */
    public int getColumn() {
        return column;
    }

    /**
     * Returns the type of the Block
     * @return BlockType - Type of the Block
     * @see BlockType
     */
    public BlockType getType() {
        return type;
    }

    /**
     * Sets the type of the Block
     * @param type The new type that will be set
     * @see BlockType
     */
    public void setType(BlockType type) {
        this.type = type;
    }

    /**
     * Returns the current health of the Block
     * @return int - Current health
     */
    public int getHealth() {
        return health;
    }

    /**
     * If the blocks health is more than 0, it applies 1 damage
     * and plays the damage sound accordingly. And if health reaches
     * 0 after damage, it decreases the number of blocks left in the current level.
     * Finally it updates the type of the block according to its new health.
     */
    public void takeDamage() {
        if (health > 0) {
            health--;
            if (health == 0) {
                blocksLeft--;
            }
        }

        updateType();
    }

    /**
     * Updates the type of the blocks according to its health.
     */
    public void updateType() {
        switch (health) {
            case (1) -> {
                setType(BlockType.BLUE);
                setStyle(BLUE_STYLE);
            }
            case (0) -> {
                setType(BlockType.EMPTY);
                setStyle(EMPTY_STYLE);
            }
        }

    }

    /**
     * Returns the number of non-destroyed blocks left in the current level
     * @return int - non-destroyed block count
     */
    public static int getBlocksLeft() {
        return blocksLeft;
    }
}

package BreakTheBlocks;


import java.util.ArrayList;
import java.util.List;

/**
 * A class that handles all game logic
 */
public class GameLogic {
    private final Block[][] board;
    FileHandler fh = new FileHandler();
    ArrayList<String[]> levelInfo;
    int clickedBlockCount = 0;
    private int currentScore = 0;
    int currentLevel;
    SoundHandler sh = new SoundHandler();


    /**
     * @param board - 2D Array of Blocks that forms the game board
     */
    public GameLogic(Block[][] board) {
        this.board = board;
    }

    /**
     * Calls methods that has to be called on every click.
     * And updates current score according to effected block count
     *
     * @param row The row index of clicked Block
     * @param col The column index of clicked Block
     * @return List - A list that contains all effected Blocks
     */
    public List<Block> onMouseClick(int row, int col) {

        List<Block> clickedBlocks = getAdjacentBlocks(row, col);
        clickedBlockCount = 0;
        for (Block block : clickedBlocks) {
            block.takeDamage();
            clickedBlockCount++;
        }
        if (clickedBlockCount > 0) {
            sh.playDamageSound();
        }
        currentScore += calculatePoint();
        return clickedBlocks;
    }

    /**
     * Calculates the point that player has earned in one click
     *
     * @return int - Amount of point that Player earned
     */
    int calculatePoint() {
        return switch (clickedBlockCount) {
            case (1) -> -3;
            case (2) -> -1;
            case (3) -> 1;
            case (4) -> 2;
            case (5) -> 4;
            default -> 0;
        };
    }

    /**
     * Gets non-diagonal and valid adjacent blocks
     *
     * @param row Row index of center Block
     * @param col Colum index of center Block
     * @return ArrayList - A list that contains appropriate Blocks
     */
    public ArrayList<Block> getAdjacentBlocks(int row, int col) {
        ArrayList<Block> list = new ArrayList<>();


        if (board[row][col].getType() != BlockType.EMPTY)
            list.add(board[row][col]);

        if (isValid(row - 1, col)
                && (board[row - 1][col].getType() != BlockType.WALL)
                && (board[row - 1][col].getType() != BlockType.EMPTY))
            list.add(board[row - 1][col]);

        if (isValid(row + 1, col)
                && (board[row + 1][col].getType() != BlockType.WALL)
                && (board[row + 1][col].getType() != BlockType.EMPTY))
            list.add(board[row + 1][col]);

        if (isValid(row, col - 1)
                && (board[row][col - 1].getType() != BlockType.WALL)
                && (board[row][col - 1].getType() != BlockType.EMPTY))
            list.add(board[row][col - 1]);

        if (isValid(row, col + 1)
                && (board[row][col + 1].getType() != BlockType.WALL)
                && (board[row][col + 1].getType() != BlockType.EMPTY))
            list.add(board[row][col + 1]);

        return list;
    }


    /**
     * Checks if the given row and column index is valid or not
     *
     * @param row Row index
     * @param col Column index
     * @return boolean - The validity of given point
     */
    boolean isValid(int row, int col) {
        return ((row >= 0 && row < board.length) && (col >= 0 && col < board[0].length));
    }

    /**
     * Gets the level info from file handler
     */
    public boolean getLevelInfo() {
        currentLevel = fh.getCurrentLevel();
        levelInfo = fh.readNextLevel();
        return !levelInfo.get(0)[0].equals("-1");
    }

    /**
     * Determines what type of Block should be created at the given
     * row and column index
     *
     * @param row Row index of the Block that being checked
     * @param col Column  index of the Block that being checked
     * @return BlockType - The type that should be created
     * @see BlockType
     */
    public BlockType checkSlot(Integer row, Integer col) {
        BlockType returnType = BlockType.WALL;
        for (String[] line : levelInfo) {
            if (line[1].equals(row.toString()) && line[2].equals(col.toString())) {
                switch (line[0]) {
                    case "Empty" -> returnType = BlockType.EMPTY;
                    case "Wood" -> returnType = BlockType.PURPLE;
                    case "Mirror" -> returnType = BlockType.BLUE;
                }
            }
        }
        return returnType;
    }

    /**
     * Returns current score
     *
     * @return int - current score
     */
    int getCurrentScore() {
        return currentScore;
    }

}

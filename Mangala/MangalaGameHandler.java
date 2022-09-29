import java.util.Random;
import java.util.Scanner;

/**
 *  Class that handles the game logic
 */
public class MangalaGameHandler {

    private Player player1;
    private Player player2;
    private Player currentPlayer;
    private Player winner = null;
    private int lastTurnIndex;
    private boolean isGameFinished = false;
    Scanner scanner = new Scanner(System.in);

    Player determineFirstPlayer() {
        Random r = new Random();
        if (r.nextBoolean()) {
            return player1;
        } else {
            return player2;
        }
    }

    Player getNextPlayer(Player currentPlayer) {
        return currentPlayer == player1 ? player2 : player1;
    }

    public MangalaGameHandler() {
        player1 = new Player("Player1");
        player2 = new Player("Player2");
        currentPlayer = determineFirstPlayer();
        System.out.println("First player is " + currentPlayer.getName() +"\n");

    }

    void playOneTurn(Player currentPlayer) {
        System.out.println(currentPlayer.getName() + "'s Turn");
        Pit[] currentPlayerPits;
        Pit currentPlayerTreasury;
        Pit[] otherPlayerPits;
        Pit otherPlayerTreasury;

        if (currentPlayer == player1) {
            currentPlayerPits = player1.getPits();
            currentPlayerTreasury = player1.getTreasury();
            otherPlayerPits = player2.getPits();
            otherPlayerTreasury = player2.getTreasury();
        } else {
            currentPlayerPits = player2.getPits();
            currentPlayerTreasury = player2.getTreasury();
            otherPlayerPits = player1.getPits();
            otherPlayerTreasury = player1.getTreasury();
        }

        int selectedIndex;


        do {
            System.out.print("Please select your pit: ");
            selectedIndex = scanner.nextInt() - 1;

            if (isSelectionNotValid(selectedIndex, currentPlayerPits)) {
                System.out.println("Please enter a valid number!");
            }
        } while (isSelectionNotValid(selectedIndex, currentPlayerPits));

        int stonesAtIndex = currentPlayerPits[selectedIndex].getStone();
        currentPlayerPits[selectedIndex].setStone(0);
        int counter = selectedIndex;


        if (stonesAtIndex == 1) {
            if (selectedIndex < 5) {  // Player didn't choose the last pit
                currentPlayerPits[selectedIndex + 1].setStone(currentPlayerPits[selectedIndex + 1].getStone() + 1);
            } else { // Player choose tha last pit
                currentPlayerTreasury.setStone(currentPlayerTreasury.getStone() + 1);
            }
            this.lastTurnIndex = selectedIndex + 1;
        } else {
            while (stonesAtIndex > 0) {
                this.lastTurnIndex = counter;
                if (counter < 6) { // Current player's own pits
                    currentPlayerPits[counter].setStone(currentPlayerPits[counter].getStone() + 1);
                    counter++;
                    stonesAtIndex--;
                } else if (counter == 6) { // Current player's treasury
                    currentPlayerTreasury.setStone(currentPlayerTreasury.getStone() + 1);
                    counter++;
                    stonesAtIndex--;
                } else if (counter != 13) { // Current player's opponents pits
                    otherPlayerPits[counter - 7].setStone(otherPlayerPits[counter - 7].getStone() + 1);
                    counter++;
                    stonesAtIndex--;
                } else {
                    counter = counter - 13;
                }
            }

        }
        endTurnControls(currentPlayerPits, currentPlayerTreasury, otherPlayerPits, otherPlayerTreasury);
    }

    void endTurnControls(Pit[] currentPlayerPits, Pit currentPlayerTreasury, Pit[] otherPlayerPits, Pit otherPlayerTreasury) {
        if (lastTurnIndex < 6) { // The player ended its turn at its own pits
            if (currentPlayerPits[lastTurnIndex].getStone() == 1 // The player ended it turn at empty pit
                    && otherPlayerPits[5 - lastTurnIndex].getStone() > 0) {
                currentPlayerTreasury.setStone(currentPlayerTreasury.getStone()
                        + currentPlayerPits[lastTurnIndex].getStone()
                        + otherPlayerPits[5 - lastTurnIndex].getStone());
                currentPlayerPits[lastTurnIndex].setStone(0);
                otherPlayerPits[5 - lastTurnIndex].setStone(0);
            }
        } else if (lastTurnIndex > 6) { // The player ended its turn at opponents pit
            if (otherPlayerPits[lastTurnIndex - 7].getStone() % 2 == 0) { // The player ended its turn at odd numbered pit therefore it became an even one
                currentPlayerTreasury.setStone(currentPlayerTreasury.getStone()
                        + otherPlayerPits[lastTurnIndex - 7].getStone());
                otherPlayerPits[lastTurnIndex - 7].setStone(0);
            }
        }
        if (checkForWinner(currentPlayerPits, currentPlayerTreasury, otherPlayerPits, otherPlayerTreasury) != null) {
            isGameFinished = true;

        } else if (lastTurnIndex != 6) { // The player didn't end the turn at its own treasury
            currentPlayer = getNextPlayer(currentPlayer);
        }
    }


    Player checkForWinner(Pit[] currentPlayerPits, Pit currentPlayerTreasury, Pit[] otherPlayerPits, Pit otherPlayerTreasury) {
        boolean isGameFinished = true;
        for (Pit pit : currentPlayerPits) {
            if (pit.getStone() != 0) {
                isGameFinished = false;
                break;
            }
        }

        if (isGameFinished) {
            int totalStoneFromOtherPlayer = 0;
            for (Pit pit : otherPlayerPits) { // Tally up all the stones of opponent
                totalStoneFromOtherPlayer += pit.getStone();
                pit.setStone(0);
            }
            currentPlayerTreasury.setStone(currentPlayerTreasury.getStone() + totalStoneFromOtherPlayer);
            if (currentPlayerTreasury.getStone() > otherPlayerTreasury.getStone()) {
                winner = currentPlayer;
            } else if (currentPlayerTreasury.getStone() < otherPlayerTreasury.getStone()) {
                winner = getNextPlayer(currentPlayer);
            }
        }
        return winner;
    }

    boolean isSelectionNotValid(int selectedIndex, Pit[] currentPlayerPits) {
        return (selectedIndex < 0 || selectedIndex > 5)
                || currentPlayerPits[selectedIndex].getStone() == 0;
    }

    void drawBoard() {

        System.out.print("\n\t");
        for (int i = 5; i >= 0; i--) { // We print in reverse so the board looks right
            System.out.print("[" + player2.getPits()[i].getStone() + "] ");
        }

        System.out.println("\n[" + player2.getTreasury().getStone() + "]\t\t\t\t\t\t\t[" + player1.getTreasury().getStone() + "]");

        System.out.print("\t");

        for (Pit pit : player1.getPits()) {
            System.out.print("[" + pit.getStone() + "] ");
        }
        System.out.println("\n\n**********************************\n");
    }

    void gameLoop() {
        while (!isGameFinished) {
            drawBoard();
            playOneTurn(currentPlayer);
        }
        drawBoard();
        if (winner != null) {
            System.out.println("Winner is " + winner.getName());
        } else { // Game is finished but there is no winner so its tie
            System.out.println("Tie");
        }

    }
}

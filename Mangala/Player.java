/**
 *  Class that handles the Player's
 */
public class Player {

    private Pit[] pits = new Pit[6];
    private Pit treasury = new Pit(0);
    private String name;

    public Player(String name){
        this.name = name;
        for (int i = 0; i < 6; i++) {
            pits[i] = new Pit();
        }
    }

    public Pit[] getPits() {
        return pits;
    }

    public Pit getTreasury() {
        return treasury;
    }

    public String getName() {
        return name;
    }




}

/**
 * A container that holds the stones
 */
public class Pit {
    private int stone;

    public Pit(){
        this.stone = 4;
    }

    public Pit(int stone) {
        this.stone = stone;
    }

    public int getStone() {
        return stone;
    }

    public void setStone(int stone) {
        this.stone = stone;
    }
}

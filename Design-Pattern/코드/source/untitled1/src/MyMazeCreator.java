import java.awt.*;

import static javax.swing.JFrame.EXIT_ON_CLOSE;

public class MyMazeCreator extends MazeCreator {

    public Door makeDoor1(Room room1, Room room2) {
        return new SecretDoor(room1, room2, "insang1");
    }
//    public Door makeDoor2(Room room1, Room room2) {
//        return new Door(room1, room2);
//    }
//    public Door makeDoor3(Room room1, Room room2) {
//        return new Door(room1, room2);
//    }
//    public Door makeDoor4(Room room1, Room room2) {
//        return new Door(room1, room2);
//    }
    public Door makeDoor5(Room room1, Room room2) {
        return new SecretDoor(room1, room2, "insang2");
    }

//    public Door makeDoor6(Room room1, Room room2) {
//        return new Door(room1, room2);
//    }

//    public Room makeRoom1(int i) {
//        return new Room(i);
//    }
//    public Room makeRoom2(int i) {
//        return new Room(i);
//    }
//    public Room makeRoom3(int i) {
//        return new Room(i);
//    }
    public Room makeRoom4(int i) {
        return new RoomWithBomb(i);
    }
//    public Room makeRoom5(int i) {
//        return new Room(i);
//    }
//    public Room makeRoom6(int i) {
//        return new Room(i);
//    }
}

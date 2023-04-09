import java.awt.*;

import static javax.swing.JFrame.EXIT_ON_CLOSE;

public class MazeCreator {
    public  Maze createMaze() {
        Maze maze = makeMaze();

        Room room1 = makeRoom1(1);
        Room room2 = makeRoom2(2);
        Room room3 = makeRoom3(3);
        Room room4 = makeRoom4(4);
        Room room5 = makeRoom5(5);
        Room room6 = makeRoom6(6);

        Door door1 = makeDoor1(room1, room3);
        Door door2 = makeDoor2(room2, room3);
        Door door3 = makeDoor3(room3, room4);
        Door door4 = makeDoor4(room2, room5);
        Door door5 = makeDoor5(room5, room6);
        Door door6 = makeDoor6(room3, room6);

        room1.setSide(Direction.NORTH, makeWall());
        room1.setSide(Direction.EAST, door1);
        room1.setSide(Direction.SOUTH, makeWall());
        room1.setSide(Direction.WEST, makeWall());

        room2.setSide(Direction.NORTH, makeWall());
        room2.setSide(Direction.EAST, door4);
        room2.setSide(Direction.SOUTH, door2);
        room2.setSide(Direction.WEST, makeWall());

        room3.setSide(Direction.NORTH, door2);
        room3.setSide(Direction.EAST, door6);
        room3.setSide(Direction.SOUTH, door3);
        room3.setSide(Direction.WEST, door1);

        room4.setSide(Direction.NORTH, door3);
        room4.setSide(Direction.EAST, makeWall());
        room4.setSide(Direction.SOUTH, makeWall());
        room4.setSide(Direction.WEST, makeWall());

        room5.setSide(Direction.NORTH, makeWall());
        room5.setSide(Direction.EAST, makeWall());
        room5.setSide(Direction.SOUTH, door5);
        room5.setSide(Direction.WEST, door4);

        room6.setSide(Direction.NORTH, door5);
        room6.setSide(Direction.EAST, makeWall());
        room6.setSide(Direction.SOUTH, makeWall());
        room6.setSide(Direction.WEST, door6);

        maze.addRoom(room1);
        maze.addRoom(room2);
        maze.addRoom(room3);
        maze.addRoom(room4);
        maze.addRoom(room5);
        maze.addRoom(room6);

        maze.setCurrentRoom(1);

        return maze;
    }

    public Maze makeMaze() {
        return new Maze();
    }

    public Wall makeWall() {
        return new Wall();
    }

    public Door makeDoor1(Room room1, Room room2) {
        return new Door(room1, room2);
    }
    public Door makeDoor2(Room room1, Room room2) {
        return new Door(room1, room2);
    }
    public Door makeDoor3(Room room1, Room room2) {
        return new Door(room1, room2);
    }
    public Door makeDoor4(Room room1, Room room2) {
        return new Door(room1, room2);
    }
    public Door makeDoor5(Room room1, Room room2) {
        return new Door(room1, room2);
    }
    public Door makeDoor6(Room room1, Room room2) {
        return new Door(room1, room2);
    }

    public Room makeRoom1(int i) {
        return new Room(i);
    }
    public Room makeRoom2(int i) {
        return new Room(i);
    }
    public Room makeRoom3(int i) {
        return new Room(i);
    }
    public Room makeRoom4(int i) {
        return new Room(i);
    }
    public Room makeRoom5(int i) {
        return new Room(i);
    }
    public Room makeRoom6(int i) {
        return new Room(i);
    }

    public static void main(String[] args) {
        MazeCreator mazeCreator = new MyMazeCreator();
        Maze maze = mazeCreator.createMaze();
        MazeController controller = MazeController.getInstance();
        controller.setMaze(maze);
        controller.setVisible(true);
        controller.setSize(new Dimension(300, 200));
        controller.setDefaultCloseOperation(EXIT_ON_CLOSE);
    }
}

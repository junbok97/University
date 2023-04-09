import java.awt.*;

import static javax.swing.JFrame.EXIT_ON_CLOSE;

public class SimpleMazeGame {
    public static Maze createMaze() {
        Maze maze = new Maze();

        Room room1 = new Room(1);
        Room room2 = new Room(2);
        Room room3 = new Room(3);
        Room room4 = new Room(4);
        Room room5 = new Room(5);
        Room room6 = new Room(6);

        Door door1 = new Door(room1, room3);
        Door door2 = new Door(room2, room3);
        Door door3 = new Door(room3, room4);
        Door door4 = new Door(room2, room5);
        Door door5 = new Door(room5, room6);
        Door door6 = new Door(room3, room6);

        room1.setSide(Direction.NORTH, new Wall());
        room1.setSide(Direction.EAST, door1);
        room1.setSide(Direction.SOUTH, new Wall());
        room1.setSide(Direction.WEST, new Wall());

        room2.setSide(Direction.NORTH, new Wall());
        room2.setSide(Direction.EAST, door4);
        room2.setSide(Direction.SOUTH, door2);
        room2.setSide(Direction.WEST, new Wall());

        room3.setSide(Direction.NORTH, door2);
        room3.setSide(Direction.EAST, door6);
        room3.setSide(Direction.SOUTH, door3);
        room3.setSide(Direction.WEST, door1);

        room4.setSide(Direction.NORTH, door3);
        room4.setSide(Direction.EAST, new Wall());
        room4.setSide(Direction.SOUTH, new Wall());
        room4.setSide(Direction.WEST, new Wall());

        room5.setSide(Direction.NORTH, new Wall());
        room5.setSide(Direction.EAST, new Wall());
        room5.setSide(Direction.SOUTH, door5);
        room5.setSide(Direction.WEST, door4);

        room6.setSide(Direction.NORTH, door5);
        room6.setSide(Direction.EAST, new Wall());
        room6.setSide(Direction.SOUTH, new Wall());
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
    public static Maze createMaze1() {
        Maze maze = new Maze();

        Room room1 = new Room(1);
        Room room2 = new Room(2);
        Room room3 = new Room(3);
        Room room4 = new RoomWithBomb(4);
        Room room5 = new Room(5);
        Room room6 = new Room(6);

        Door door1 = new SecretDoor(room1, room3, "insang1");
        Door door2 = new Door(room2, room3);
        Door door3 = new Door(room3, room4);
        Door door4 = new Door(room2, room5);
        Door door5 = new SecretDoor(room5, room6, "insang2");
        Door door6 = new Door(room3, room6);

        room1.setSide(Direction.NORTH, new Wall());
        room1.setSide(Direction.EAST, door1);
        room1.setSide(Direction.SOUTH, new Wall());
        room1.setSide(Direction.WEST, new Wall());

        room2.setSide(Direction.NORTH, new Wall());
        room2.setSide(Direction.EAST, door4);
        room2.setSide(Direction.SOUTH, door2);
        room2.setSide(Direction.WEST, new Wall());

        room3.setSide(Direction.NORTH, door2);
        room3.setSide(Direction.EAST, door6);
        room3.setSide(Direction.SOUTH, door3);
        room3.setSide(Direction.WEST, door1);

        room4.setSide(Direction.NORTH, door3);
        room4.setSide(Direction.EAST, new Wall());
        room4.setSide(Direction.SOUTH, new Wall());
        room4.setSide(Direction.WEST, new Wall());

        room5.setSide(Direction.NORTH, new Wall());
        room5.setSide(Direction.EAST, new Wall());
        room5.setSide(Direction.SOUTH, door5);
        room5.setSide(Direction.WEST, door4);

        room6.setSide(Direction.NORTH, door5);
        room6.setSide(Direction.EAST, new Wall());
        room6.setSide(Direction.SOUTH, new Wall());
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

    public static void main(String[] args) {
        Maze maze = createMaze();
        MazeController controller = MazeController.getInstance();
        controller.setMaze(maze);
        controller.setVisible(true);
        controller.setSize(new Dimension(300, 200));
        controller.setDefaultCloseOperation(EXIT_ON_CLOSE);
    }
}

public class Translator implements Observer {
    private Professor professor;

    public void setProfessor(Professor professor) {
        this.professor = professor;
    }
    public void update() {
        String content = professor.getContent();
        translate(content);
    }

    private void translate(String content) {
        System.out.print("Translator: "+content);
    }
}

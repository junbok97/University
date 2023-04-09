public class Main {
    public static void main(String[] args) {
        Professor prof1 = new Professor("insang");
        Recorder recorder = new Recorder();
        prof1.attach(recorder);
        recorder.setProfessor(prof1);
        Translator translator = new Translator();
        prof1.attach(translator);
        translator.setProfessor(prof1);
        prof1.talk("observer");
        prof1.talk("builder");

        Blog blog = new Blog();
        User user = new User("gildong");
        blog.attach(user);
        user.setMyBlog(blog);
        blog.changeContent("Hello");
        blog.changeContent("World");

    }
}

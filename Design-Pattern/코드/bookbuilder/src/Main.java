public class Main {
    public static void main(String[] args) {

        Book book = new Book.BookBuilder(1L, "isbn1234").author("insang chung").
                pages(360).category("CE").title("Design Pattern").build();


    }
}

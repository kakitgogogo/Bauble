import java.util.*;

public class IOTest {
	public static void main(String[] args) {
		Scanner in = new Scanner(System.in);
		String name = in.nextLine();
		int age = in.nextInt();
		float weight = in.nextFloat();

		System.out.println(name + " " + age + " " + weight);

		while(in.hasNext()) {
			String word = in.next();
			System.out.println(word);
		}

		in = new Scanner("11.0 22.0 33.0 haha");
		while(in.hasNextDouble()) {
			double x = in.nextDouble();
			System.out.println(x);
		}

	}
}
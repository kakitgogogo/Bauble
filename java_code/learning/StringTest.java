import java.util.Arrays;
import java.util.regex.*;

public class StringTest {
	public static void main(String[] args) {
		String s1 = "Hello";
		System.out.println(s1.toUpperCase());

		System.out.println(s1.replaceAll("l", "L"));

		String s2 = "   A,B,C   ";
		System.out.println(s2.trim());
		String[] sa = s2.trim().split(",");
		for(String s:sa) {
			System.out.println(s);
		}

		String s3 = String.format("%05d, %s", 12, "op");
		System.out.println(s3);

		String regex = "(;|,)";
		String [] sa1 = "a;b,c;d".split(regex);
		for(String s:sa1) {
			System.out.println(s);
		}

		String s4 = "abcd";
		byte[]  bytes = s4.getBytes();
		System.out.println(bytes);
		//String s5 = new String(s4.getBytes("ISO-8859-1"), "UTF-8");
		//System.out.println(s5);

		StringBuffer s6 = new StringBuffer("abc");
		s6.append("d");
		s6.insert(1, "0");
		System.out.println(s6);
	}
}
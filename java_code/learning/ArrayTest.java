import java.util.Arrays;

public class ArrayTest{
	public static void main(String args[]){
    	int sample[];
    	sample = new int[8];
    	System.out.println(sample[7]);

    	for(int s:sample){
    		System.out.print(s);
    	}

    	System.out.print("\n");

    	int table[][] = {{1}, {1,2}, {1,2,3}};
    	for(int i = 0; i < table.length; ++i){
    		for(int j = 0; j < table[i].length; ++j){
    			System.out.print(table[i][j]);
    		}
    		System.out.print("\n");
    	}

    	char s1[] = {'H', 'e', 'l', 'l', 'o'};
    	s1 = Arrays.copyOf(s1, 8);

    	System.out.println(s1);

    	int a[] = {1, 4, 2, 3, 0};
    	int pos = Arrays.binarySearch(a, 3);
    	System.out.println(pos);

    	Arrays.sort(a, 1, 4);
    	for(int num:a){
    		System.out.print(num);
    	}
    	System.out.println();

    }
}

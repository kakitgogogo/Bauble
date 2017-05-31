import java.util.*;

class Vehicle {
	int type;
	void drive() {
		System.out.println("Run! " + this.getClass());
	}
}

class Car extends Vehicle {

}

class Bus extends Vehicle {

}

public class ClassTest {
	public static void main(String[] args) {
		Bus bus = new Bus();
		bus.drive();
		Car car = new Car();
		car.drive();
	}
}
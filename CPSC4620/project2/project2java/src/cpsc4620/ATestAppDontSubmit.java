// package cpsc4620; //UNCOMMENT FOR SUBMISSION

// import java.util.*;
import java.io.IOException;
import java.sql.SQLException;

public class ATestAppDontSubmit {

    public static void main(String[] args) {
        System.out.println("Welcome to DBNinja!");

        // test connection
        java.sql.Connection conn;
        try {
            System.out.println("Testing database connection...");
            conn = DBConnector.make_connection();
            System.out.println("Connection successful!");
            conn.close();
            System.out.println("Connection closed!");

        } catch (SQLException e) {
            System.out.println("Connection failed!");
            System.out.println("Message     : " + e.getMessage());
            return;
        } catch (IOException e) {
            System.out.println("Connection failed!");
            System.out.println("Message     : " + e.getMessage());
            return;
        }
        System.out.println();

        try {
            // -------------------------------
            // Test 1: Get all customers
            // -------------------------------
            System.out.println("Customers:");
            for (Customer c : DBNinja.getCustomerList()) {
                System.out.println(c.getFName() + " " + c.getLName());
            }
            // -------------------------------
            // Test 2: Find customer
            // -------------------------------
            Customer c = DBNinja.findCustomerByPhone("8642545861");
            if (c != null) {
                System.out.println("Found: " + c.getFName());
            }
            // -------------------------------
            // Test 3: Get toppings
            // -------------------------------
            System.out.println("\nToppings:");
            for (Topping t : DBNinja.getToppingList()) {
                System.out.println(t.getTopName());
            }
            // -------------------------------
            // Test 4: Get discounts
            // -------------------------------
            System.out.println("\nDiscounts:");
            for (Discount d : DBNinja.getDiscountList()) {
                System.out.println(d.getDiscountName());
            }
            // -------------------------------
            // Test 5: Get orders
            // -------------------------------
            System.out.println("\nOrders:");
            for (Order o : DBNinja.getOrders(3)) {
                System.out.println("Order ID: " + o.getOrderID());
                // for (Pizza p : o.getPizzas()) {
                //     System.out.println("  Pizza: " + p.getSize() + " " + p.getCrustType());
                // }
            }
            // -------------------------------
            // Test 6: Reports
            // -------------------------------
            DBNinja.printToppingReport();
            DBNinja.printProfitByPizzaReport();
            DBNinja.printProfitByOrderTypeReport();

            // place a test delivery order
            /*
            (int orderID, int custID, String date, double custPrice, double busPrice, boolean isComplete, String address) 
            Pizza(int pizzaID, String size, String crustType, int orderID, String pizzaState, String pizzaDate,
            double custPrice, double busPrice)
             */
            Order o = new DeliveryOrder(10, 3, "2026-01-03", 20.0, 10.0, false, "123\tMain St\tAnytown\tCA\t90210");
            Pizza p = new Pizza(23, DBNinja.size_m, DBNinja.crust_thin, 10, "pizzastate", "2026-01-03", 20.0, 10.0);
            Topping t = DBNinja.getToppingList().get(0); // get first topping
            p.getToppings().add(t);
            o.addPizza(p);
            DBNinja.addOrder(o);

            // place a pickup order
            Order o2 = new PickupOrder(11, 3, "2026-01-03", 15.0, 7.0, false, false);
            Pizza p2 = new Pizza(24, DBNinja.size_s, DBNinja.crust_orig, 11, "pizzastate", "2026-01-03", 15.0, 7.0);
            Topping t2Topping = DBNinja.getToppingList().get(1); // get second topping
            t2Topping.setDoubled(true); // make it a double
            p2.getToppings().add(t2Topping);
            o2.addPizza(p2);
            DBNinja.addOrder(o2);

            // get most recent order, print it
            Order recentOrder = DBNinja.getLastOrder();
            System.out.println("\nMost recent order:");
            System.out.println(recentOrder);

            DBNinja.completeOrder(recentOrder.getOrderID(), DBNinja.order_state.PREPARED);

            // same thing
            recentOrder = DBNinja.getLastOrder();
            System.out.println("\nMost recent order after marking prepared:");
            System.out.println(recentOrder);

            DBNinja.completeOrder(recentOrder.getOrderID(), DBNinja.order_state.PICKEDUP);

            // once more
            recentOrder = DBNinja.getLastOrder();
            System.out.println("\nMost recent order after marking completed:");
            System.out.println(recentOrder);



            // place a test dine-in order. add a discount. 
            /*
            (int orderID, int custID, String date, double custPrice, double busPrice, boolean isComplete, int tablenum) {
            */
            Order o3 = new DineinOrder(12, 3, "2026-01-03", 30.0, 15.0, false, 5);
            Pizza p3 = new Pizza(25, DBNinja.size_l, DBNinja.crust_pan, 12, "pizzastate", "2026-01-03", 30.0, 15.0);
            Topping t3 = DBNinja.getToppingList().get(2); // get third topping
            p3.getToppings().add(t3);
            o3.addPizza(p3);
            o3.addDiscount(new Discount(1, "Employee", 10.0, true));
            DBNinja.addOrder(o3);

            System.out.println("\nDine-in order with discount:");
            System.out.println(o3);
            recentOrder = DBNinja.getLastOrder();
            System.out.println("\nMost recent order after adding dine-in order with discount:");
            System.out.println(recentOrder);
            // get discount list of order
            System.out.println("\nDiscounts on most recent order:");
            for (Discount d : recentOrder.getDiscountList()) {
                System.out.println(d.getDiscountName() + " - " + (d.isPercent() ? d.getAmount() + "%" : "$" + d.getAmount()));
            }


            // list inventory report
            // nvm ill just use mysql to look at the table lmao
            
            

            // -------------------------------
            // TEST F: Date parsing test
            // -------------------------------
            System.out.println("\n\nTEST F: Date Parsing");

            try {
                java.text.SimpleDateFormat sdf = new java.text.SimpleDateFormat("yyyy-MM-dd");
                java.util.Date d = sdf.parse("2026-01-03");
                System.out.println("Parsed date OK: " + d);
            } catch (Exception e) {
                System.out.println("Date parsing FAILED");
            }

        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}

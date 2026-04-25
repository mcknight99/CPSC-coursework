package cpsc4620;

import java.io.IOException;
import java.sql.*;
import java.util.*;

/*
 * This file is where you will implement the methods needed to support this application.
 * You will write the code to retrieve and save information to the database and use that
 * information to build the various objects required by the applicaiton.
 * 
 * The class has several hard coded static variables used for the connection, you will need to
 * change those to your connection information
 * 
 * This class also has static string variables for pickup, delivery and dine-in. 
 * DO NOT change these constant values.
 * 
 * You can add any helper methods you need, but you must implement all the methods
 * in this class and use them to complete the project.  The autograder will rely on
 * these methods being implemented, so do not delete them or alter their method
 * signatures.
 * 
 * Make sure you properly open and close your DB connections in any method that
 * requires access to the DB.
 * Use the connect_to_db below to open your connection in DBConnector.
 * What is opened must be closed!
 */

 /*
 * A utility class to help add and retrieve information from the database
 */
public final class DBNinja {

    private static Connection conn;

    // DO NOT change these variables!
    public final static String pickup = "pickup";
    public final static String delivery = "delivery";
    public final static String dine_in = "dinein";

    public final static String size_s = "Small";
    public final static String size_m = "Medium";
    public final static String size_l = "Large";
    public final static String size_xl = "XLarge";

    public final static String crust_thin = "Thin";
    public final static String crust_orig = "Original";
    public final static String crust_pan = "Pan";
    public final static String crust_gf = "Gluten-Free";

    public enum order_state {
        PREPARED,
        DELIVERED,
        PICKEDUP
    }

    private static boolean connect_to_db() throws SQLException, IOException {

        try {
            conn = DBConnector.make_connection();
            return true;
        } catch (SQLException e) {
            return false;
        } catch (IOException e) {
            return false;
        }

    }

    public static void addOrder(Order o) throws SQLException, IOException {
        /*
		 * add code to add the order to the DB. Remember that we're not just
		 * adding the order to the order DB table, but we're also recording
		 * the necessary data for the delivery, dinein, pickup, pizzas, toppings
		 * on pizzas, order discounts and pizza discounts.
		 * 
		 * This is a KEY method as it must store all the data in the Order object
		 * in the database and make sure all the tables are correctly linked.
		 * 
		 * Remember, if the order is for Dine In, there is no customer...
		 * so the cusomter id coming from the Order object will be -1.
		 * 
         * Add a new order to the database: You must be able to add a new order with pizzas to the
database. Remember, there's more to adding an order to the DB than just creating an order and
sending it to the database via SQL. You need to create all the pizzas that go in the order, update
topping inventories as necessary, checking to make sure a topping inventory never goes
negative, and apply discounts to pizzas and orders. This means you'll be updating more than just
the order table for this operation. You can assume that a topping can only be added to a pizza 1
time and all the Pizzas in an Order must have the same date/time stamp as the Order.

         */

        connect_to_db();

        String sql = "INSERT INTO ordertable (customer_CustID, ordertable_OrderType, ordertable_OrderDateTime, ordertable_CustPrice, ordertable_BusPrice, ordertable_IsComplete) VALUES (?, ?, ?, ?, ?, ?)";

        PreparedStatement ps = conn.prepareStatement(sql, Statement.RETURN_GENERATED_KEYS);

        // Handle dine-in (no customer)
        if (o.getCustID() == -1) {
            ps.setNull(1, Types.INTEGER);
        } else {
            ps.setInt(1, o.getCustID());
        }

        ps.setString(2, o.getOrderType());
        ps.setString(3, o.getDate()); // 
        ps.setDouble(4, o.getCustPrice());
        ps.setDouble(5, o.getBusPrice());
        ps.setBoolean(6, false);

        ps.executeUpdate();

        ResultSet rs = ps.getGeneratedKeys();
        int orderID = -1;

        if (rs.next()) {
            orderID = rs.getInt(1);
        }

        // Handle order type tables
        if (o.getOrderType().equals(delivery)) {

            String dSQL = "INSERT INTO delivery VALUES (?, ?, ?, ?, ?, ?, ?)";
            PreparedStatement dps = conn.prepareStatement(dSQL);
            // orderid, housenum, street, city, state, zip, isdelivered
            dps.setInt(1, orderID);
            String[] addrParts = ((DeliveryOrder) o).getAddress().split("\t");
            dps.setInt(2, Integer.parseInt(addrParts[0])); // house num
            dps.setString(3, addrParts[1]); // street
            dps.setString(4, addrParts[2]); // city
            dps.setString(5, addrParts[3]); // state
            dps.setInt(6, Integer.parseInt(addrParts[4])); // zip
            dps.setBoolean(7, false);

            dps.executeUpdate();

        } else if (o.getOrderType().equals(pickup)) {

            String pSQL = "INSERT INTO pickup VALUES (?, ?)";
            PreparedStatement pps = conn.prepareStatement(pSQL);

            pps.setInt(1, orderID);
            pps.setInt(2, 0);

            pps.executeUpdate();

        } else if (o.getOrderType().equals(dine_in)) {

            String diSQL = "INSERT INTO dinein VALUES (?, ?)";
            PreparedStatement dips = conn.prepareStatement(diSQL);

            dips.setInt(1, orderID);
            dips.setInt(2, ((DineinOrder) o).getTableNum());

            dips.executeUpdate();
        }

        // Add order discounts
        for (Discount d : o.getDiscountList()) {

            String zdSQL = "INSERT INTO order_discount VALUES (?, ?)";
            PreparedStatement zdps = conn.prepareStatement(zdSQL);

            // orderid and discountid
            zdps.setInt(1, orderID);
            zdps.setInt(2, d.getDiscountID());

            zdps.executeUpdate();
        }

        // Add pizzas
        for (Pizza p : o.getPizzaList()) {
            // date string to util date
            java.util.Date date = null;
            try {
                java.text.SimpleDateFormat sdf = new java.text.SimpleDateFormat("yyyy-MM-dd");
                date = sdf.parse(o.getDate().split(" ")[0]);

            } catch (java.text.ParseException e) {
                e.printStackTrace();
                // handle parse exception
            }
            addPizza(date, orderID, p); // this will also add toppings and pizza discounts, and update inventory
        }
    }

    static int addPizza(java.util.Date d, int orderID, Pizza p) throws SQLException, IOException {
        /*
		 * Add the code needed to insert the pizza into into the database.
		 * Keep in mind you must also add the pizza discounts and toppings 
		 * associated with the pizza.
		 * 
		 * NOTE: there is a Date object passed into this method so that the Order
		 * and ALL its Pizzas can be assigned the same DTS.
		 * 
		 * This method returns the id of the pizza just added.
		 * 
         */

        connect_to_db();

        String sql = "INSERT INTO pizza (pizza_Size, pizza_CrustType, ordertable_OrderID, pizza_PizzaState, pizza_PizzaDate, pizza_CustPrice, pizza_BusPrice) VALUES (?, ?, ?, ?, ?, ?, ?)";

        PreparedStatement ps = conn.prepareStatement(sql, Statement.RETURN_GENERATED_KEYS);

        ps.setString(1, p.getSize());
        ps.setString(2, p.getCrustType());
        ps.setInt(3, orderID);
        ps.setString(4, p.getPizzaState()); 
        ps.setTimestamp(5, new Timestamp(d.getTime()));
        ps.setDouble(6, p.getCustPrice());
        ps.setDouble(7, p.getBusPrice());

        ps.executeUpdate();

        ResultSet rs = ps.getGeneratedKeys();
        int pizzaID = -1;

        if (rs.next()) {
            pizzaID = rs.getInt(1);
        }

        // Add toppings
        for (Topping t : p.getToppings()) {

            String tSQL = "INSERT INTO pizza_topping (pizza_PizzaID, topping_TopID, pizza_topping_IsDouble) VALUES (?, ?, ?)";

            PreparedStatement tps = conn.prepareStatement(tSQL);

            tps.setInt(1, pizzaID);
            tps.setInt(2, t.getTopID());
            tps.setInt(3, t.getDoubled() ? 1 : 0);

            tps.executeUpdate();

            // update topping count
            // determine correct base amount based on pizza size
            double baseAmount = 0;

            if (p.getSize().equalsIgnoreCase(size_s)) {
                baseAmount = t.getSmallAMT();
            } else if (p.getSize().equalsIgnoreCase(size_m)) {
                baseAmount = t.getMedAMT();
            } else if (p.getSize().equalsIgnoreCase(size_l)) {
                baseAmount = t.getLgAMT();
            } else if (p.getSize().equalsIgnoreCase(size_xl)) {
                baseAmount = t.getXLAMT();
            }

            // double topping = double the amount
            double quantityUsed = t.getDoubled() ? baseAmount * 2 : baseAmount;
            quantityUsed = Math.round(quantityUsed); // round to nearest whole number // if the amount i take off ends up being too high i might want to round before doubling then
            // holy moly this actually did the trick ^

            // update topping inventory
            String iSQL = "UPDATE topping SET topping_CurINVT = topping_CurINVT - ? WHERE topping_TopID = ?";
            PreparedStatement ips = conn.prepareStatement(iSQL);

            ips.setDouble(1, quantityUsed);
            ips.setInt(2, t.getTopID());

            ips.executeUpdate();
        }

        // Add pizza discounts
        for (Discount dsc : p.getDiscounts()) {

            String dSQL = "INSERT INTO pizza_discount (pizza_PizzaID, discount_DiscountID) VALUES (?, ?)";

            PreparedStatement dps = conn.prepareStatement(dSQL);

            dps.setInt(1, pizzaID);
            dps.setInt(2, dsc.getDiscountID());

            dps.executeUpdate();
        }

        conn.close();
        return pizzaID;

        // return -1;
    }

    public static int addCustomer(Customer c) throws SQLException, IOException {
        /*
		 * This method adds a new customer to the database.
		 * 
         */

        //  return -1;
        connect_to_db();

        String sql = "INSERT INTO customer (customer_FName, customer_LName, customer_PhoneNum) VALUES (?, ?, ?)";

        PreparedStatement ps = conn.prepareStatement(sql, Statement.RETURN_GENERATED_KEYS);

        // Set values from the Customer object
        ps.setString(1, c.getFName());
        ps.setString(2, c.getLName());
        ps.setString(3, c.getPhone());

        ps.executeUpdate();

        // Get auto-generated ID
        ResultSet rs = ps.getGeneratedKeys();
        int id = -1;

        if (rs.next()) {
            id = rs.getInt(1);
        }

        conn.close();
        return id;
    }

    public static void completeOrder(int OrderID, order_state newState) throws SQLException, IOException {
        /*
		 * Mark that order as complete in the database.
		 * Note: if an order is complete, this means all the pizzas are complete as well.
		 * However, it does not mean that the order has been delivered or picked up!
		 *
		 * For newState = PREPARED: mark the order and all associated pizza's as completed
		 * For newState = DELIVERED: mark the delivery status
		 * FOR newState = PICKEDUP: mark the pickup status
		 * 
         */

        connect_to_db();

        if (newState == order_state.PREPARED) {

            String sql = "UPDATE ordertable SET ordertable_IsComplete = 1 WHERE ordertable_OrderID = ?";
            PreparedStatement ps = conn.prepareStatement(sql);
            ps.setInt(1, OrderID);
            ps.executeUpdate();

            String psql = "UPDATE pizza SET pizza_PizzaState = 'completed' WHERE ordertable_OrderID = ?";
            PreparedStatement pps = conn.prepareStatement(psql);
            pps.setInt(1, OrderID);
            pps.executeUpdate();

        } else if (newState == order_state.DELIVERED) {

            String sql = "UPDATE delivery SET delivery_IsDelivered = 1 WHERE ordertable_OrderID = ?";
            PreparedStatement ps = conn.prepareStatement(sql);
            ps.setInt(1, OrderID);
            ps.executeUpdate();

        } else if (newState == order_state.PICKEDUP) {

            String sql = "UPDATE pickup SET pickup_IsPickedUp = 1 WHERE ordertable_OrderID = ?";
            PreparedStatement ps = conn.prepareStatement(sql);
            ps.setInt(1, OrderID);
            ps.executeUpdate();
        }

        conn.close();

    }

    public static ArrayList<Order> getOrders(int status) throws SQLException, IOException {
        /*
	 * Return an ArrayList of orders.
	 * 	status   == 1 => return a list of open (ie oder is not completed)
	 *           == 2 => return a list of completed orders (ie order is complete)
	 *           == 3 => return a list of all the orders
	 * Remember that in Java, we account for supertypes and subtypes
	 * which means that when we create an arrayList of orders, that really
	 * means we have an arrayList of dineinOrders, deliveryOrders, and pickupOrders.
	 *
	 * You must fully populate the Order object, this includes order discounts,
	 * and pizzas along with the toppings and discounts associated with them.
	 * 
	 * Don't forget to order the data according to their order sequence, ie, order 1, order 2, etc.
	 *
         */
        // return null;

        connect_to_db();

        ArrayList<Order> list = new ArrayList<>();

        String sql = "SELECT * FROM ordertable o "
                + " LEFT JOIN delivery d ON o.ordertable_OrderID = d.ordertable_OrderID"
                + " LEFT JOIN pickup p ON o.ordertable_OrderID = p.ordertable_OrderID"
                + " LEFT JOIN dinein di ON o.ordertable_OrderID = di.ordertable_OrderID";

        if (status == 1) {
            sql += " WHERE o.ordertable_IsComplete = 0";
        } else if (status == 2) {
            sql += " WHERE o.ordertable_IsComplete = 1";
        }

        sql += " ORDER BY o.ordertable_OrderID";

        PreparedStatement ps = conn.prepareStatement(sql);
        ResultSet rs = ps.executeQuery();

        while (rs.next()) {

            // check type
            String type = rs.getString("ordertable_OrderType");

            Order o;

            if (type.equalsIgnoreCase(delivery)) {

                int houseNum = rs.getInt("delivery_HouseNum");
                String street = rs.getString("delivery_Street");
                String city = rs.getString("delivery_City");
                String state = rs.getString("delivery_State");
                int zip = rs.getInt("delivery_Zip");

                String addr = houseNum + "\t" + street + "\t" + city + "\t" + state + "\t" + zip;

                o = new DeliveryOrder(
                        rs.getInt("ordertable_OrderID"),
                        rs.getInt("customer_CustID"),
                        rs.getString("ordertable_OrderDateTime"),
                        rs.getDouble("ordertable_CustPrice"),
                        rs.getDouble("ordertable_BusPrice"),
                        rs.getBoolean("ordertable_IsComplete"),
                        rs.getBoolean("delivery_IsDelivered"),
                        addr
                );

            } else if (type.equalsIgnoreCase(pickup)) {

                o = new PickupOrder(
                        rs.getInt("ordertable_OrderID"),
                        rs.getInt("customer_CustID"),
                        rs.getString("ordertable_OrderDateTime"),
                        rs.getDouble("ordertable_CustPrice"),
                        rs.getDouble("ordertable_BusPrice"),
                        rs.getBoolean("pickup_IsPickedUp"),
                        rs.getBoolean("ordertable_IsComplete")
                );

            } else if (type.equalsIgnoreCase(dine_in)) {
                // dine-in (if exists OR just use Order)
                o = new DineinOrder(
                        rs.getInt("ordertable_OrderID"),
                        rs.getInt("customer_CustID"),
                        rs.getString("ordertable_OrderDateTime"),
                        rs.getDouble("ordertable_CustPrice"),
                        rs.getDouble("ordertable_BusPrice"),
                        rs.getBoolean("ordertable_IsComplete"),
                        rs.getInt("dinein_TableNum")
                );
            } else {
                // default to Order if type is unrecognized // shouldn't happen if data is correct
                o = new Order(
                        rs.getInt("ordertable_OrderID"),
                        rs.getInt("customer_CustID"),
                        type,
                        rs.getString("ordertable_OrderDateTime"),
                        rs.getDouble("ordertable_CustPrice"),
                        rs.getDouble("ordertable_BusPrice"),
                        rs.getBoolean("ordertable_IsComplete")
                );
            }

            // attach pizzas + discounts
            o.setPizzaList(getPizzas(o));
            o.setDiscountList(getDiscounts(o));

            list.add(o);
        }

        conn.close();
        return list;
    }

    public static Order getLastOrder() throws SQLException, IOException {
        /*
		 * Query the database for the LAST order added
		 * then return an Order object for that order.
		 * NOTE...there will ALWAYS be a "last order"!
         */
        //  return null;
        connect_to_db();

        String sql = "SELECT * FROM ordertable o"
                + " LEFT JOIN delivery d ON o.ordertable_OrderID = d.ordertable_OrderID"
                + " LEFT JOIN pickup p ON o.ordertable_OrderID = p.ordertable_OrderID"
                + " LEFT JOIN dinein di ON o.ordertable_OrderID = di.ordertable_OrderID"
                + " ORDER BY o.ordertable_OrderID DESC LIMIT 1;";

        PreparedStatement ps = conn.prepareStatement(sql);
        ResultSet rs = ps.executeQuery();

        Order o = null;

        if (rs.next()) {
            int id = rs.getInt("ordertable_OrderID");
            int custID = rs.getInt("customer_CustID");
            String type = rs.getString("ordertable_OrderType");
            String date = rs.getString("ordertable_OrderDateTime");
            double custPrice = rs.getDouble("ordertable_CustPrice");
            double busPrice = rs.getDouble("ordertable_BusPrice");
            boolean isComplete = rs.getBoolean("ordertable_IsComplete");

            // Create appropriate subtype
            if (type.equalsIgnoreCase(delivery)) {

                int houseNum = rs.getInt("delivery_HouseNum");
                String street = rs.getString("delivery_Street");
                String city = rs.getString("delivery_City");
                String state = rs.getString("delivery_State");
                int zip = rs.getInt("delivery_Zip");

                String addr = houseNum + "\t" + street + "\t" + city + "\t" + state + "\t" + zip;

                o = new DeliveryOrder(
                        rs.getInt("ordertable_OrderID"),
                        rs.getInt("customer_CustID"),
                        rs.getString("ordertable_OrderDateTime"),
                        rs.getDouble("ordertable_CustPrice"),
                        rs.getDouble("ordertable_BusPrice"),
                        rs.getBoolean("ordertable_IsComplete"),
                        rs.getBoolean("delivery_IsDelivered"),
                        addr
                );
            } else if (type.equalsIgnoreCase(pickup)) {

                o = new PickupOrder(
                        id, custID, date, custPrice, busPrice, rs.getBoolean("pickup_IsPickedUp"), isComplete
                );

            } else if (type.equalsIgnoreCase(dine_in)) {
                // dine-in (if exists OR just use Order)
                o = new DineinOrder(
                        id, custID, date, custPrice, busPrice, isComplete, rs.getInt("dinein_TableNum")
                );
            } else {
                // default to Order if type is unrecognized // shouldn't happen if data is correct
                o = new Order(
                        id, custID, type, date, custPrice, busPrice, isComplete
                );
            }

            //  Populate children
            o.setPizzaList(getPizzas(o));
            o.setDiscountList(getDiscounts(o));
        }

        conn.close();
        return o;
    }

    public static ArrayList<Order> getOrdersByDate(String date) throws SQLException, IOException {
        /*
		 * Query the database for ALL the orders placed on a specific date
		 * and return a list of those orders.
		 *  
         */
        //  return null;
        connect_to_db();

        ArrayList<Order> orders = new ArrayList<>();

        String sql = "SELECT * FROM ordertable o"
                + " LEFT JOIN delivery d ON o.ordertable_OrderID = d.ordertable_OrderID"
                + " LEFT JOIN pickup p ON o.ordertable_OrderID = p.ordertable_OrderID"
                + " LEFT JOIN dinein di ON o.ordertable_OrderID = di.ordertable_OrderID"
                + " WHERE DATE(o.ordertable_OrderDateTime) = ? ORDER BY o.ordertable_OrderID;";

        PreparedStatement ps = conn.prepareStatement(sql);
        ps.setString(1, date);

        ResultSet rs = ps.executeQuery();

        while (rs.next()) {
            int id = rs.getInt("ordertable_OrderID");
            int custID = rs.getInt("customer_CustID");
            String type = rs.getString("ordertable_OrderType");
            double custPrice = rs.getDouble("ordertable_CustPrice");
            double busPrice = rs.getDouble("ordertable_BusPrice");
            boolean isComplete = rs.getBoolean("ordertable_IsComplete");

            Order o;

            // Create appropriate subtype
            if (type.equalsIgnoreCase(delivery)) {

                int houseNum = rs.getInt("delivery_HouseNum");
                String street = rs.getString("delivery_Street");
                String city = rs.getString("delivery_City");
                String state = rs.getString("delivery_State");
                int zip = rs.getInt("delivery_Zip");

                String addr = houseNum + "\t" + street + "\t" + city + "\t" + state + "\t" + zip;

                o = new DeliveryOrder(
                        id, custID, date, custPrice, busPrice, isComplete, rs.getBoolean("delivery_IsDelivered"), addr
                );

            } else if (type.equalsIgnoreCase(pickup)) {

                o = new PickupOrder(
                        id, custID, date, custPrice, busPrice, rs.getBoolean("pickup_IsPickedUp"), isComplete
                );

            } else if (type.equalsIgnoreCase(dine_in)) {
                // dine-in (if exists OR just use Order)
                o = new DineinOrder(
                        id, custID, date, custPrice, busPrice, isComplete, rs.getInt("dinein_TableNum")
                );
            } else {
                // default to Order if type is unrecognized
                o = new Order(
                        id, custID, type, date, custPrice, busPrice, isComplete
                );
            }

            // full populate
            o.setPizzaList(getPizzas(o));
            o.setDiscountList(getDiscounts(o));

            orders.add(o);
        }

        conn.close();
        return orders;
    }

    public static ArrayList<Discount> getDiscountList() throws SQLException, IOException {
        /* 
		 * Query the database for all the available discounts and 
		 * return them in an arrayList of discounts ordered by discount name.
		 * 
         */
        // return null;

        connect_to_db();

        ArrayList<Discount> list = new ArrayList<>();

        String sql = "SELECT * FROM discount ORDER BY discount_DiscountName";
        PreparedStatement ps = conn.prepareStatement(sql);
        ResultSet rs = ps.executeQuery();

        while (rs.next()) {
            Discount d = new Discount(
                    rs.getInt("discount_DiscountID"),
                    rs.getString("discount_DiscountName"),
                    rs.getDouble("discount_Amount"),
                    rs.getBoolean("discount_IsPercent")
            );
            list.add(d);
        }

        conn.close();
        return list;
    }

    public static Discount findDiscountByName(String name) throws SQLException, IOException {
        /*
		 * Query the database for a discount using it's name.
		 * If found, then return an OrderDiscount object for the discount.
		 * If it's not found....then return null
		 *  
         */
        //  return null;
        connect_to_db();

        String sql = "SELECT * FROM discount WHERE discount_DiscountName = ?;";
        PreparedStatement ps = conn.prepareStatement(sql);
        ps.setString(1, name);

        ResultSet rs = ps.executeQuery();

        if (rs.next()) {
            Discount d = new Discount(
                    rs.getInt("discount_DiscountID"),
                    rs.getString("discount_DiscountName"),
                    rs.getDouble("discount_Amount"),
                    rs.getBoolean("discount_IsPercent")
            );
            conn.close();
            return d;
        }

        conn.close();
        return null;
    }

    public static ArrayList<Customer> getCustomerList() throws SQLException, IOException {
        /*
		 * Query the data for all the customers and return an arrayList of all the customers. 
		 * Don't forget to order the data coming from the database appropriately.
		 * 
         */
        // return null;
        connect_to_db();

        ArrayList<Customer> customers = new ArrayList<>();

        String sql = "SELECT * FROM customer ORDER BY customer_LName, customer_FName;";
        PreparedStatement ps = conn.prepareStatement(sql);
        ResultSet rs = ps.executeQuery();

        while (rs.next()) {
            Customer c = new Customer(
                    rs.getInt("customer_CustID"),
                    rs.getString("customer_FName"),
                    rs.getString("customer_LName"),
                    rs.getString("customer_PhoneNum")
            );
            customers.add(c);
        }

        conn.close();
        return customers;
    }

    public static Customer findCustomerByPhone(String phoneNumber) throws SQLException, IOException {
        /*
		 * Query the database for a customer using a phone number.
		 * If found, then return a Customer object for the customer.
		 * If it's not found....then return null
		 *  
         */
        //  return null;

        connect_to_db();

        String sql = "SELECT * FROM customer WHERE customer_PhoneNum = ?";
        PreparedStatement ps = conn.prepareStatement(sql);
        ps.setString(1, phoneNumber);

        ResultSet rs = ps.executeQuery();

        if (rs.next()) {
            Customer c = new Customer(
                    rs.getInt("customer_CustID"),
                    rs.getString("customer_FName"),
                    rs.getString("customer_LName"),
                    rs.getString("customer_PhoneNum")
            );
            conn.close();
            return c;
        }

        conn.close();
        return null;

    }

    public static String getCustomerName(int CustID) throws SQLException, IOException {
        /*
		 * COMPLETED...WORKING Example!
		 * 
		 * This is a helper method to fetch and format the name of a customer
		 * based on a customer ID. This is an example of how to interact with
		 * your database from Java.  
		 * 
		 * Notice how the connection to the DB made at the start of the 
		 *
         */

        connect_to_db();

        /* 
		 * an example query using a constructed string...
		 * remember, this style of query construction could be subject to sql injection attacks!
		 * 
         */
        String cname1 = "";
        String cname2 = "";
        String query = "Select customer_FName, customer_LName From customer WHERE customer_CustID=" + CustID + ";";
        Statement stmt = conn.createStatement();
        ResultSet rset = stmt.executeQuery(query);

        while (rset.next()) {
            cname1 = rset.getString(1) + " " + rset.getString(2);
        }

        /* 
		* an BETTER example of the same query using a prepared statement...
		* with exception handling
		* 
         */
        try {
            PreparedStatement os;
            ResultSet rset2;
            String query2;
            query2 = "Select customer_FName, customer_LName From customer WHERE customer_CustID=?;";
            os = conn.prepareStatement(query2);
            os.setInt(1, CustID);
            rset2 = os.executeQuery();
            while (rset2.next()) {
                cname2 = rset2.getString("customer_FName") + " " + rset2.getString("customer_LName"); // note the use of field names in the getSting methods
            }
        } catch (SQLException e) {
            e.printStackTrace();
            // process the error or re-raise the exception to a higher level
        }

        conn.close();

        return cname1;
        // OR
        // return cname2;

    }

    public static ArrayList<Topping> getToppingList() throws SQLException, IOException {
        /*
		 * Query the database for the aviable toppings and 
		 * return an arrayList of all the available toppings. 
		 * Don't forget to order the data coming from the database appropriately.
		 * 
         */
        // return null;

        connect_to_db();

        ArrayList<Topping> list = new ArrayList<>();

        String sql = "SELECT * FROM topping ORDER BY topping_TopName";
        PreparedStatement ps = conn.prepareStatement(sql);
        ResultSet rs = ps.executeQuery();

        while (rs.next()) {
            Topping t = new Topping(
                    rs.getInt("topping_TopID"),
                    rs.getString("topping_TopName"),
                    rs.getDouble("topping_SmallAMT"),
                    rs.getDouble("topping_MedAMT"),
                    rs.getDouble("topping_LgAMT"),
                    rs.getDouble("topping_XLAMT"),
                    rs.getDouble("topping_CustPrice"),
                    rs.getDouble("topping_BusPrice"),
                    rs.getInt("topping_MinINVT"),
                    rs.getInt("topping_CurINVT")
            );
            list.add(t);
        }

        conn.close();
        return list;
    }

    public static Topping findToppingByName(String name) throws SQLException, IOException {
        /*
		 * Query the database for the topping using it's name.
		 * If found, then return a Topping object for the topping.
		 * If it's not found....then return null
		 *  
         */
        //  return null;
        connect_to_db();

        String sql = "SELECT * FROM topping WHERE topping_TopName = ?;";
        PreparedStatement ps = conn.prepareStatement(sql);
        ps.setString(1, name);

        ResultSet rs = ps.executeQuery();

        if (rs.next()) {
            Topping t = new Topping(
                    rs.getInt("topping_TopID"),
                    rs.getString("topping_TopName"),
                    rs.getDouble("topping_SmallAMT"),
                    rs.getDouble("topping_MedAMT"),
                    rs.getDouble("topping_LgAMT"),
                    rs.getDouble("topping_XLAMT"),
                    rs.getDouble("topping_CustPrice"),
                    rs.getDouble("topping_BusPrice"),
                    rs.getInt("topping_MinINVT"),
                    rs.getInt("topping_CurINVT")
            );
            conn.close();
            return t;
        }

        conn.close();
        return null;
    }

    public static ArrayList<Topping> getToppingsOnPizza(Pizza p) throws SQLException, IOException {
        /* 
		 * This method builds an ArrayList of the toppings ON a pizza.
		 * The list can then be added to the Pizza object elsewhere in the
         */

        // return null;	
        connect_to_db();

        ArrayList<Topping> toppings = new ArrayList<>();

        String sql = "SELECT t.*, pt.pizza_topping_IsDouble FROM pizza_topping pt JOIN topping t ON pt.topping_TopID = t.topping_TopID WHERE pt.pizza_PizzaID = ?;";
        PreparedStatement ps = conn.prepareStatement(sql);
        ps.setInt(1, p.getPizzaID());

        ResultSet rs = ps.executeQuery();

        while (rs.next()) {
            Topping t = new Topping(
                    rs.getInt("topping_TopID"),
                    rs.getString("topping_TopName"),
                    rs.getDouble("topping_SmallAMT"),
                    rs.getDouble("topping_MedAMT"),
                    rs.getDouble("topping_LgAMT"),
                    rs.getDouble("topping_XLAMT"),
                    rs.getDouble("topping_CustPrice"),
                    rs.getDouble("topping_BusPrice"),
                    rs.getInt("topping_MinINVT"),
                    rs.getInt("topping_CurINVT")
            );

            // set double topping flag
            t.setDoubled(rs.getInt("pizza_topping_IsDouble") == 1);

            toppings.add(t);
        }

        conn.close();
        return toppings;
    }

    public static void addToInventory(int toppingID, double quantity) throws SQLException, IOException {
        /*
		 * Updates the quantity of the topping in the database by the amount specified.
		 * 
		 * */
        connect_to_db();

        String sql = "UPDATE topping SET topping_CurINVT = topping_CurINVT + ? WHERE topping_TopID = ?;";
        PreparedStatement ps = conn.prepareStatement(sql);
        ps.setDouble(1, quantity);
        ps.setInt(2, toppingID);

        ps.executeUpdate();

        conn.close();
    }

    public static ArrayList<Pizza> getPizzas(Order o) throws SQLException, IOException {
        /*
		 * Build an ArrayList of all the Pizzas associated with the Order.
		 * 
         */
        // return null;

        connect_to_db();

        ArrayList<Pizza> list = new ArrayList<>();

        String sql = "SELECT * FROM pizza WHERE ordertable_OrderID=?";
        PreparedStatement ps = conn.prepareStatement(sql);
        ps.setInt(1, o.getOrderID());

        ResultSet rs = ps.executeQuery();

        while (rs.next()) {

            Pizza p = new Pizza(
                    rs.getInt("pizza_PizzaID"),
                    rs.getString("pizza_Size"),
                    rs.getString("pizza_CrustType"),
                    rs.getInt("ordertable_OrderID"),
                    rs.getString("pizza_PizzaState"),
                    rs.getString("pizza_PizzaDate"),
                    rs.getDouble("pizza_CustPrice"),
                    rs.getDouble("pizza_BusPrice")
            );

            // attach toppings + discounts
            p.setToppings(getToppingsOnPizza(p));
            p.setDiscounts(getDiscounts(p));

            list.add(p);
        }

        conn.close();
        return list;
    }

    public static ArrayList<Discount> getDiscounts(Order o) throws SQLException, IOException {
        /* 
		 * Build an array list of all the Discounts associted with the Order.
		 * 
         */

        // return null;
        connect_to_db();

        ArrayList<Discount> discounts = new ArrayList<>();

        String sql = "SELECT d.* FROM order_discount od JOIN discount d ON od.discount_DiscountID = d.discount_DiscountID WHERE od.ordertable_OrderID = ?;";

        PreparedStatement ps = conn.prepareStatement(sql);
        ps.setInt(1, o.getOrderID());

        ResultSet rs = ps.executeQuery();

        while (rs.next()) {
            Discount d = new Discount(
                    rs.getInt("discount_DiscountID"),
                    rs.getString("discount_DiscountName"),
                    rs.getDouble("discount_Amount"),
                    rs.getBoolean("discount_IsPercent")
            );
            discounts.add(d);
        }

        conn.close();
        return discounts;
    }

    public static ArrayList<Discount> getDiscounts(Pizza p) throws SQLException, IOException {
        /* 
		 * Build an array list of all the Discounts associted with the Pizza.
		 * 
         */

        // return null;
        connect_to_db();

        ArrayList<Discount> discounts = new ArrayList<>();

        String sql = "SELECT d.* FROM pizza_discount pd JOIN discount d ON pd.discount_DiscountID = d.discount_DiscountID WHERE pd.pizza_PizzaID = ?;";
        PreparedStatement ps = conn.prepareStatement(sql);
        ps.setInt(1, p.getPizzaID());

        ResultSet rs = ps.executeQuery();

        while (rs.next()) {
            Discount d = new Discount(
                    rs.getInt("discount_DiscountID"),
                    rs.getString("discount_DiscountName"),
                    rs.getDouble("discount_Amount"),
                    rs.getBoolean("discount_IsPercent")
            );
            discounts.add(d);
        }

        conn.close();
        return discounts;
    }

    public static double getBaseCustPrice(String size, String crust) throws SQLException, IOException {
        /* 
		 * Query the database fro the base customer price for that size and crust pizza.
		 * 
         */
        // return 0.0;

        connect_to_db();

        String sql = "SELECT baseprice_CustPrice FROM baseprice WHERE baseprice_Size=? AND baseprice_CrustType=?";
        PreparedStatement ps = conn.prepareStatement(sql);

        ps.setString(1, size);
        ps.setString(2, crust);

        ResultSet rs = ps.executeQuery();

        double price = 0;
        if (rs.next()) {
            price = rs.getDouble(1);
        }

        conn.close();
        return price;
    }

    public static double getBaseBusPrice(String size, String crust) throws SQLException, IOException {
        /* 
		 * Query the database fro the base business price for that size and crust pizza.
		 * 
         */
        // return 0.0;

        connect_to_db();

        String sql = "SELECT baseprice_BusPrice FROM baseprice WHERE baseprice_Size=? AND baseprice_CrustType=?";
        PreparedStatement ps = conn.prepareStatement(sql);

        ps.setString(1, size);
        ps.setString(2, crust);

        ResultSet rs = ps.executeQuery();

        double price = 0;
        if (rs.next()) {
            price = rs.getDouble(1);
        }

        conn.close();
        return price;
    }

    // private static int duct_tape(String topping, int off) throws SQLException, IOException {
    //     // helper function to fix unknown view issues. 
    //     // return slightly higher values for specific toppings to match expected values
    //     // pepperoni should +4
    //     // regular cheese should +1
    //     // four cheese blend should +2
    //     // ham should +1
    //     // rest should +0

    //     switch (topping.toLowerCase()) {
    //         case "pepperoni":
    //             return 4;
    //         case "regular cheese":
    //             return 1;
    //         case "four cheese blend":
    //             return 2;
    //         case "ham":
    //             return 1;
    //         default:
    //             return 0;
    //         }
    //     }
        

    public static void printToppingReport() throws SQLException, IOException {
        /*
		 * Prints the ToppingPopularity view. Remember that this view
		 * needs to exist in your DB, so be sure you've run your createViews.sql
		 * files on your testing DB if you haven't already.
		 * 
		 * The result should be readable and sorted as indicated in the prompt.
		 * 
		 * HINT: You need to match the expected output EXACTLY....I would suggest
		 * you look at the printf method (rather that the simple print of println).
		 * It operates the same in Java as it does in C and will make your code
		 * better.
		 * 
         */

        connect_to_db();

        String sql
                = "SELECT t.topping_TopName AS Topping, "
                + "COUNT(pt.topping_TopID) AS ToppingCount "
                + "FROM topping t "
                + "LEFT JOIN pizza_topping pt ON t.topping_TopID = pt.topping_TopID "
                + "GROUP BY t.topping_TopID, t.topping_TopName "
                + "ORDER BY ToppingCount DESC";

        PreparedStatement ps = conn.prepareStatement(sql);
        ResultSet rs = ps.executeQuery();

        System.out.printf("%-20s%-20s\n", "Topping", "Topping Count");
        System.out.printf("%-20s%-20s\n", "-------", "-------------");

        while (rs.next()) {
            String topping = rs.getString("Topping");
            int count = rs.getInt("ToppingCount");
            // count += duct_tape(topping, count); // workaround for unknown view issues // not going to do it because it feels ingenuine

            System.out.printf("%-20s%-20d\n", topping, count);
        }

        conn.close();
    }

    public static void printProfitByPizzaReport() throws SQLException, IOException {
        /*
		 * Prints the ProfitByPizza view. Remember that this view
		 * needs to exist in your DB, so be sure you've run your createViews.sql
		 * files on your testing DB if you haven't already.
		 * 
		 * The result should be readable and sorted as indicated in the prompt.
		 * 
		 * HINT: You need to match the expected output EXACTLY....I would suggest
		 * you look at the printf method (rather that the simple print of println).
		 * It operates the same in Java as it does in C and will make your code
		 * better.
		 * 
         */
        connect_to_db();

        /*
     * Extract month/year using DATE_FORMAT
     * Aggregate profit per pizza type
         */
        String sql
                = "SELECT p.pizza_Size AS Size, "
                + "p.pizza_CrustType AS Crust, "
                + "SUM(p.pizza_CustPrice - p.pizza_BusPrice) AS Profit, "
                + "DATE_FORMAT(MAX(o.ordertable_OrderDateTime), '%c/%Y') AS OrderMonth "
                + "FROM pizza p "
                + "JOIN ordertable o ON p.ordertable_OrderID = o.ordertable_OrderID "
                + "GROUP BY p.pizza_Size, p.pizza_CrustType "
                + "ORDER BY Profit ASC";

        PreparedStatement ps = conn.prepareStatement(sql);
        ResultSet rs = ps.executeQuery();

        System.out.printf("%-20s%-20s%-20s%-20s\n",
                "Pizza Size", "Pizza Crust", "Profit", "Last Order Date");
        System.out.printf("%-20s%-20s%-20s%-20s\n",
                "----------", "-----------", "------", "---------------");

        while (rs.next()) {
            String size = rs.getString("Size");
            String crust = rs.getString("Crust");
            double profit = rs.getDouble("Profit");
            String month = rs.getString("OrderMonth");

            System.out.printf("%-20s%-20s%-20.2f%-20s\n",
                    size, crust, profit, month);
        }

        conn.close();
    }

    public static void printProfitByOrderTypeReport() throws SQLException, IOException {
        /*
		 * Prints the ProfitByOrderType view. Remember that this view
		 * needs to exist in your DB, so be sure you've run your createViews.sql
		 * files on your testing DB if you haven't already.
		 * 
		 * The result should be readable and sorted as indicated in the prompt.
		 *
		 * HINT: You need to match the expected output EXACTLY....I would suggest
		 * you look at the printf method (rather that the simple print of println).
		 * It operates the same in Java as it does in C and will make your code
		 * better.
		 * 
         */
        connect_to_db();

        String sql
                = "SELECT o.ordertable_OrderType AS CustomerType, "
                + "DATE_FORMAT(o.ordertable_OrderDateTime, '%c/%Y') AS OrderMonth, "
                + "SUM(o.ordertable_CustPrice) AS TotalOrderPrice, "
                + "SUM(o.ordertable_BusPrice) AS TotalOrderCost, "
                + "SUM(o.ordertable_CustPrice - o.ordertable_BusPrice) AS Profit "
                + "FROM ordertable o "
                + "GROUP BY CustomerType, OrderMonth "
                + "ORDER BY Profit ASC";

        PreparedStatement ps = conn.prepareStatement(sql);
        ResultSet rs = ps.executeQuery();

        double grandPrice = 0;
        double grandCost = 0;
        double grandProfit = 0;

        System.out.printf("%-20s%-20s%-20s%-20s%-20s\n",
                "Customer Type", "Order Month", "Total Order Price", "Total Order Cost", "Profit");
        System.out.printf("%-20s%-20s%-20s%-20s%-20s\n",
                "-------------", "-----------", "-----------------", "----------------", "------");

        while (rs.next()) {
            String type = rs.getString("CustomerType");
            String month = rs.getString("OrderMonth");
            double price = rs.getDouble("TotalOrderPrice");
            double cost = rs.getDouble("TotalOrderCost");
            double profit = rs.getDouble("Profit");

            grandPrice += price;
            grandCost += cost;
            grandProfit += profit;

            System.out.printf("%-20s%-20s%-20.2f%-20.2f%-20.2f\n",
                    type, month, price, cost, profit);
        }

        // Grand total row (IMPORTANT for matching expected output)
        System.out.printf("%-20s%-20s%-20.2f%-20.2f%-20.2f\n",
                "", "Grand Total", grandPrice, grandCost, grandProfit);

        conn.close();
    }

    /*
	 * These private methods help get the individual components of an SQL datetime object. 
	 * You're welcome to keep them or remove them....but they are usefull!
     */
    private static int getYear(String date)// assumes date format 'YYYY-MM-DD HH:mm:ss'
    {
        return Integer.parseInt(date.substring(0, 4));
    }

    private static int getMonth(String date)// assumes date format 'YYYY-MM-DD HH:mm:ss'
    {
        return Integer.parseInt(date.substring(5, 7));
    }

    private static int getDay(String date)// assumes date format 'YYYY-MM-DD HH:mm:ss'
    {
        return Integer.parseInt(date.substring(8, 10));
    }

    public static boolean checkDate(int year, int month, int day, String dateOfOrder) {
        if (getYear(dateOfOrder) > year) {
            return true;
        } else if (getYear(dateOfOrder) < year) {
            return false;
        } else {
            if (getMonth(dateOfOrder) > month) {
                return true;
            } else if (getMonth(dateOfOrder) < month) {
                return false;
            } else {
                if (getDay(dateOfOrder) >= day) {
                    return true;
                } else {
                    return false;
                }
            }
        }
    }

}

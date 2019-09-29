package paystation.domain;

import java.util.HashMap;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Scanner;
import paystation.domain.PayStationImpl.DayOfWeek;

/**
 *
 * @author TuanNguyen, coizioc
 */
public class PayStationMain {
   
    public static void main(String[] args) throws IllegalCoinException {
        Scanner sc = new Scanner(System.in);
        PayStationImpl paystation = new PayStationImpl();   //create paystation object
        
        while(true) {   //Paystation Menu options
            System.out.println("PAYSTATION MACHINE");
            System.out.println("==================");
            System.out.println(" 1. Deposit Coins");
            System.out.println(" 2. Display");
            System.out.println(" 3. Buy Ticket");
            System.out.println(" 4. Cancel");
            System.out.println(" 5. Change Rate Strategy");
            System.out.println(" 6. Exit PayStation");
            System.out.print("Select an Option: ");
            
            int option = sc.nextInt();  // get input
            switch(option) {
                case 1:
                    depositCoins(paystation); // execute to deposit coin in machine
                    break;
                case 2:
                    display(paystation); // execute to display current coin insert so far
                    break;
                case 3:
                    buyTicket(paystation); // execute to buy ticket and display time bought
                    break;
                case 4:
                    cancel(paystation); // execute to cancel and return coin innsert so far
                    break;
                case 5:
                    changeRateStrategy(paystation); //execute to change rate strategy : 3 rate strategys
                    break;
                case 6:
                    return; // execute to exit paystation program
               
                default:
                    System.out.println("Please enter a valid option."); // user enter invalid coin print the statement
            }
            System.out.println("");
        }
    }
    
    /*
        Function to allow user to deposit coin by enter (q: quarter, n : nickel, d: dime)
    */
   
    public static void depositCoins(PayStationImpl paystation) {
        while(true) {
            System.out.print("Please enter coin (nickel, dime, quarter)\n"
                    + "or 'exit' to stop entering coins: ");
            Scanner sc = new Scanner(System.in);
            String deposit = sc.nextLine();
            if("".equals(deposit)) {
                System.out.println("Please enter a value.");
                continue;
            }
            
            int value = 0;
            switch(deposit.charAt(0)) {
                case 'n':  //nickel 
                    value = 5;
                    break;
                case 'd': // dime
                    value = 10;
                    break;
                case 'q': //quarter
                    value = 25;
                    break;
                case 'e': //enter other letter , ask user to re-enter 
                    return;
                default:
                    System.out.println("Invalid coin."); 
                    continue;
            }
            try {
                paystation.addPayment(value); //get coin insert
                System.out.println("Deposited "+ value + " cents");
            } catch(IllegalCoinException e) { //throw and exeception when enter invalid coin
                System.out.println("Invalid coin value: " + deposit);
            }
        }
    }
    /*
      Function to allow display amount of time bought so far
    */

    private static void display(PayStationImpl paystation) throws IllegalCoinException{
        System.out.println("Time bought: " + paystation.readDisplay() + " min"); // show time bought 
       
    }
    
    /*
    Function to allow buy parking ticket  , and display receipt contains time bought
    */
    private static void buyTicket(PayStationImpl paystation) throws IllegalCoinException {
        Receipt receipt; //create receipt object
        receipt = paystation.buy();  // receipt object set to buy() fuction in paystation
        System.out.println("Parking time receipt: " + receipt.value() + " min"); // display time bought
    }
    /*
    Function to allow cancel coin , return all coins that deposit in machine 
    */
    private static void cancel(PayStationImpl paystation) throws IllegalCoinException{
        Map<Integer, Integer> map = paystation.cancel(); // create Map <Integer,Integer> to do cancel 
        
        System.out.println("Returning:");
        String outString = "";
        int totalValue = 0;
        for(Entry<Integer, Integer> entry : map.entrySet()) { // loop through map to get key and value
            int coinValue = entry.getKey(); // set entry key to coin value 
            int numCoins = entry.getValue(); // set entry value to number of coin 
            totalValue += coinValue * numCoins; // set total amount of time to equal coin value * number of coin 
            
            outString += numCoins + " ";
            switch(coinValue) {
                case 5:
                    outString += "nickel";
                    break;
                case 10:
                    outString += "dime";
                    break;
                case 25:
                    outString += "quarter";
                    break;
                default:
                    // Debug value.
                    outString += coinValue;
            }
            if(numCoins != 1) { // print number of coin insert
                outString += "s"; // ex:  1s quarter, 2s  dime, 3s nickel 
            }
            outString += "\n";
        }
       
        System.out.println(outString);
        System.out.println("Total Value: " + totalValue); 
    }
    /*
    Function to allow change Rate strategy : linear, progressive, and alternating 
    */
    private static void changeRateStrategy(PayStationImpl paystation) {
        RateStrategy linearRateStrategy = new LinearRateStrategy(); // create linearRateStrategy onject 
        RateStrategy progressiveRateStrategy = new ProgressiveRateStrategy(); //create progressiveRateStrategy object
        Map<DayOfWeek, RateStrategy> alternatingRateStrategy = new HashMap<DayOfWeek, RateStrategy>() { // use map to determine the day of week
            {
                put(DayOfWeek.SATURDAY, progressiveRateStrategy); // if it's Saturaday and Sunday then use progressive rate strategy 
                put(DayOfWeek.SUNDAY, progressiveRateStrategy);
            }
        };
        
        String password = "1234"; // create a password to prevent other user to access and change rate strategy
        Scanner sc = new Scanner(System.in);
        
        System.out.print("Please enter the password: "); // enter correct password: 1234 to change rate strategy
        String userInput = sc.nextLine();
        if(!userInput.equals(password)) {
            System.out.println("Incorrect password.");
            return;
        }
        int userChoice = 0;
        
        while(true) { // change rate strategy options
            System.out.println("Please enter a number to set the rate strategy:");
            System.out.println(" 1. Linear Rate Strategy");
            System.out.println(" 2. Progressive Rate Strategy");
            System.out.println(" 3. Alternating Rate Strategy");
            
            userChoice = sc.nextInt();
            switch(userChoice) {
                case 1: // enter 1 to calculate linear rate strategy
                    paystation.setRateStrategy(linearRateStrategy);
                    System.out.println("Rate strategy set to linear rate strategy.");
                    return;
                case 2: // enter 2 to calculate progressive rate strategy
                    paystation.setRateStrategy(progressiveRateStrategy);
                    System.out.println("Rate strategy set to progressive rate strategy.");
                    return;
                case 3: // enter 3 to calculate alternating rate strategy
                    paystation.setRateStrategy(alternatingRateStrategy);
                    System.out.println("Rate strategy set to alternating rate strategy.");
                    return;
                default: // enter other numbers print invalid option
                    System.out.println("Please enter a valid rate strategy.");
            }
        }  
        
        
    }
   
}

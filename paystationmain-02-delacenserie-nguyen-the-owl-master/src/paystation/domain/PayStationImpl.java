package paystation.domain;
import java.util.*;

/**
 * Implementation of the pay station.
 *
 * Responsibilities:
 *
 * 1) Accept payment; 
 * 2) Calculate parking time based on payment; 
 * 3) Know earning, parking time bought; 
 * 4) Issue receipts; 
 * 5) Handle buy and cancel events.
 *
 * This source code is from the book "Flexible, Reliable Software: Using
 * Patterns and Agile Development" published 2010 by CRC Press. Author: Henrik B
 * Christensen Computer Science Department Aarhus University
 *
 * This source code is provided WITHOUT ANY WARRANTY either expressed or
 * implied. You may study, use, modify, and distribute it for non-commercial
 * purposes. For any commercial use, see http://www.baerbak.com/
 */
public class PayStationImpl implements PayStation {
    public enum DayOfWeek { // declare day of week
        SUNDAY, MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY
    }
    
    private int insertedSoFar;
    private int timeBought;
    private Map coinMap = new HashMap();
    private boolean nickleBool = false;
    private boolean dimeBool = false;
    private boolean quarterBool = false;
    private final Calendar calendar = Calendar.getInstance(); //use build in function to create calendar
    private RateStrategy[] rs = new RateStrategy[7];
    
    /**
     * PayStationImpl constructor. Default is LinearRateStrategy every day of the week.
     */
    public PayStationImpl() {
        for(int i = 0; i < rs.length; i++) {
            rs[i] = new LinearRateStrategy();
        }
    }
    
    /**
     * PayStationImpl constructor. Set RateStrategy to rs every day of the week.
     * @param rs : Rate strategy to be used for the whole week.
     */
    public PayStationImpl(RateStrategy rs) {
        for(int i = 0; i < this.rs.length; i++) {
            this.rs[i] = rs;
        }
    }
    
    /**
     * PayStationImpl constructor. Set RateStrategy to strategy on day dayOfWeek.
     * @param rs : Map of rate strategies to override the default rate strategy.
     */
    public PayStationImpl(Map<DayOfWeek,RateStrategy> rs) {
        // Set default RateStrategy to LinearRateStrategy.
        for(int i = 0; i < this.rs.length; i++) {
            this.rs[i] = new LinearRateStrategy();
        }
        
        // Replace defaults with key-value pairs in entry.
        rs.entrySet().forEach((entry) -> {
            DayOfWeek dayOfWeek = entry.getKey();
            RateStrategy strategy = entry.getValue();
            this.rs[dayOfWeek.ordinal()] = strategy;
        });
    }
    
    /**
     * Gets the current day of the week.
     * @return An integer representing the current day of the week,
     * where 0 is Sunday, and so on.
     */
    private int getDayOfWeek() {
        // .get(Calendar.DAY_OF_WEEK) returns an integer from 1-7 from Sunday to Saturday.
        // Thus, to convert it to an index of DayOfWeek, we subtract one from it.
        return calendar.get(Calendar.DAY_OF_WEEK) - 1;
    }
    
    /**
     * Sets the rate strategy for the whole week.
     * @param rs : Rate strategy to set for the whole week.
     */
    public void setRateStrategy(RateStrategy rs) {
        for(int i = 0; i < this.rs.length; i++) {
            this.rs[i] = rs;
        }
    }
    
    /**
     * Sets the rate strategy for a particular day of the week.
     * @param dayOfWeek : the day of the week on which to set the rate strategy.
     * @param rs : The rate strategy.
     */
    public void setRateStrategy(DayOfWeek dayOfWeek, RateStrategy rs) {
        this.rs[dayOfWeek.ordinal()] = rs;
    }
    
    /**
     * Sets the rate strategy for a particular day of the week.
     * @param rs : Map of rate strategies to override the default rate strategy..
     */
    public void setRateStrategy(Map<DayOfWeek, RateStrategy> rs) {
        // Set default RateStrategy to LinearRateStrategy.
        for(int i = 0; i < this.rs.length; i++) {
            this.rs[i] = new LinearRateStrategy();
        }
        
        // Replace defaults with key-value pairs in entry.
        rs.entrySet().forEach((entry) -> {
            DayOfWeek dayOfWeek = entry.getKey();
            RateStrategy strategy = entry.getValue();
            setRateStrategy(dayOfWeek, strategy);
        });
    }
    
    /**
     * Gets the names of the rate strategies as strings.
     * @return An array of strings containing the rate strategies.
     */
    public String[] getRateStrategies() {
        String[] rateStrategyStrings = new String[7];
        for(int i = 0; i < rs.length; i++) {
            rateStrategyStrings[i] = rs[i].getClass().getSimpleName();
        }
        return rateStrategyStrings;
    }
    
    @Override
    public void addPayment(int coinValue)
            throws IllegalCoinException {

        switch (coinValue) {
            case 5:
                if(!nickleBool)
                {
                    coinMap.put(5, 1);
                    nickleBool = true;
                }
                else
                {
                    coinMap.put(5, (int)coinMap.get(5) + 1);
                    
                }
                break;
            case 10:
                if(!dimeBool)
                {
                    coinMap.put(10, 1);
                    dimeBool = true;
                }
                else
                {
                    coinMap.put(10, (int)coinMap.get(10) + 1);
                }
                break;
            case 25:
                if(!quarterBool)
                {
                    coinMap.put(25, 1);
                    quarterBool = true;
                }
                else
                {
                    coinMap.put(25, (int)coinMap.get(25) + 1);
                }
                break;
            default:
                throw new IllegalCoinException("Invalid coin: " + coinValue);
        }
        insertedSoFar += coinValue;
        timeBought = rs[getDayOfWeek()].calculateTime(insertedSoFar);
    }

    @Override
    public int readDisplay() {
        return timeBought;
    }

    @Override
    public Receipt buy() {
        Receipt r = new ReceiptImpl(timeBought);
        reset();
        return r;
    }

    @Override
    public Map<Integer, Integer> cancel() 
    {
        Map tempMap =  new HashMap();
        tempMap.putAll(coinMap);
        reset();
        return tempMap;
    }
    
    private void reset() {
        timeBought = insertedSoFar = 0;
        nickleBool = false;
        dimeBool = false;
        quarterBool = false;
        coinMap.clear();
    }
    
    @Override
    public int empty()
    {
        int total = insertedSoFar;
        insertedSoFar = 0;
        return total;
    }
}

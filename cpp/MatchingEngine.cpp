#include <algorithm>      // 
#include <functional>     // 
#include <list>           // 
#include <map>            //
#include <mutex>          // 
#include <optional>       // 
#include <unordered_map>  // 
#include <vector>         // 
#include <iterator>
using namespace std;

enum class OrderType
{
    BUY,
    SELL
};


#include <bits/stdc++.h>
using namespace std;

enum class OrderType {
    BUY,
    SELL
};

class MatchingEngine {
public:
    struct Trade {
        int sellOrderId;
        int buyOrderId;
        int quantity;
        int price;
    };

    struct Order {
        int orderId;
        int price;
        int quantity;
        OrderType type;

        Order(int id, int p, int q, OrderType t)
            : orderId(id), price(p), quantity(q), type(t) {}
    };

private:
    using OrderList = list<Order>;

    struct OrderLocation {
        int price;
        OrderType type;
        OrderList::iterator position;

        OrderLocation(int p, OrderType t, OrderList::iterator it)
            : price(p), type(t), position(it) {}
    };

    mutable mutex mtx;

    unordered_map<int, OrderLocation> orderMap;
    map<int, OrderList, greater<int>> buyOrders;
    map<int, OrderList> sellOrders;
    vector<Trade> trades;


    void addSellOrderLocked(const Order& order) {
        int quantity = order.quantity;

        while (quantity > 0 &&
               !buyOrders.empty() &&
               buyOrders.begin()->first >= order.price) {

            auto levelIt = buyOrders.begin();
            auto& orders = levelIt->second;
            auto orderIt = orders.begin();
            Order& resting = *orderIt;

            int tradedQty = min(quantity, resting.quantity);

            trades.push_back({
                order.orderId,
                resting.orderId,
                tradedQty,
                resting.price
            });

            quantity -= tradedQty;
            resting.quantity -= tradedQty;

            if (resting.quantity == 0) {
                orderMap.erase(resting.orderId);
                orders.erase(orderIt);
            }

            if (orders.empty()) {
                buyOrders.erase(levelIt);
            }
        }

        if (quantity > 0) {
            auto& orders = sellOrders[order.price];
            orders.emplace_back(
                order.orderId, order.price, quantity, OrderType::SELL
            );

            auto position = prev(orders.end());
            orderMap.emplace(
                order.orderId,
                OrderLocation(order.price, OrderType::SELL, position)
            );
        }
    }

    void addBuyOrderLocked(const Order& order) {
        int quantity = order.quantity;

        while (quantity > 0 &&
               !sellOrders.empty() &&
               sellOrders.begin()->first <= order.price) {

            auto levelIt = sellOrders.begin();
            auto& orders = levelIt->second;
            auto orderIt = orders.begin();
            Order& resting = *orderIt;

            int tradedQty = min(quantity, resting.quantity);

            trades.push_back({
                resting.orderId,
                order.orderId,
                tradedQty,
                resting.price
            });

            quantity -= tradedQty;
            resting.quantity -= tradedQty;

            if (resting.quantity == 0) {
                orderMap.erase(resting.orderId);
                orders.erase(orderIt);
            }

            if (orders.empty()) {
                sellOrders.erase(levelIt);
            }
        }

        if (quantity > 0) {
            auto& orders = buyOrders[order.price];
            orders.emplace_back(
                order.orderId, order.price, quantity, OrderType::BUY
            );

            auto position = prev(orders.end());
            orderMap.emplace(
                order.orderId,
                OrderLocation(order.price, OrderType::BUY, position)
            );
        }
    }

public:
    bool addOrder(const Order& order) {
        lock_guard<mutex> lock(mtx);

        if (order.orderId < 0 ||
            order.price <= 0 ||
            order.quantity <= 0 ||
            (order.type != OrderType::BUY &&
             order.type != OrderType::SELL) ||
            orderMap.count(order.orderId)) {
            return false;
        }

        if (order.type == OrderType::SELL) {
            addSellOrderLocked(order);
        } else {
            addBuyOrderLocked(order);
        }

        return true;
    }

    bool cancelOrder(int orderId) {
        lock_guard<mutex> lock(mtx);

        auto it = orderMap.find(orderId);
        if (it == orderMap.end()) {
            return false;
        }

        int price = it->second.price;
        OrderType type = it->second.type;
        auto position = it->second.position;

        if (type == OrderType::BUY) {
            auto levelIt = buyOrders.find(price);
            auto& orders = levelIt->second;

            orders.erase(position);

            if (orders.empty()) {
                buyOrders.erase(levelIt);
            }
        } else {
            auto levelIt = sellOrders.find(price);
            auto& orders = levelIt->second;

            orders.erase(position);

            if (orders.empty()) {
                sellOrders.erase(levelIt);
            }
        }

        orderMap.erase(it);
        return true;
    }

    vector<Trade> getTrades() const {
        lock_guard<mutex> lock(mtx);
        return trades; // Return a snapshot.
    }

    vector<Order> getOrderBook(OrderType side) const {
        lock_guard<mutex> lock(mtx);

        vector<Order> result;

        if (side == OrderType::BUY) {
            for (const auto& [price, orders] : buyOrders) {
                result.insert(result.end(), orders.begin(), orders.end());
            }
        } else if (side == OrderType::SELL) {
            for (const auto& [price, orders] : sellOrders) {
                result.insert(result.end(), orders.begin(), orders.end());
            }
        }

        return result;
    }
};

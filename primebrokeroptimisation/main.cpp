#include <iostream>
#include <unordered_map>
#include <stdexcept>
#include <string>

// Assume  k available accounts 2 <= k <= 5
// Assume each account comes with predefined cash and security positions
// assume each account starts in compliance with constraints

// trade attributes
// assume all trades are either corporate or government bonds
// each trade is:
// buy/sell
// security name
// price
// securitytype [corporate bond, government bond]
// quantity
// accrued interest

enum class SecurityType {
    CorporateBond,
    GovernmentBond
};

enum class Currency {
    USD,
    EUR,
    GBP,
    JPY
};

struct Trade {
    std::string securityName;
    double price;
    unsigned int quantity;
    bool isBuy;
    SecurityType securityType;
    double accruedInterest;
    Currency currency;
};

class Account {
private:
    std::string name;

    std::unordered_map<Currency, double> cashBalances;

    std::unordered_map<std::string, unsigned int> holdings;

    std::unordered_map<std::string, double> accruedInterest;

public:
    Account(const std::string& accountName) : name(accountName) {}

    void deposit(Currency currency, double amount) {
        cashBalances[currency] += amount;
    }

    double getCash(Currency currency) const {
        auto it = cashBalances.find(currency);
        return (it != cashBalances.end()) ? it->second : 0.0;
    }

    unsigned int getHolding(const std::string& securityName) const {
        auto it = holdings.find(securityName);
        return (it != holdings.end()) ? it->second : 0;
    }

};

int main() {
    Account account("Alpha");

    account.deposit(Currency::USD, 100000.0);

    String securityName = "CorpBond123";
    price = 980.0;
    unsigned int quantity = 50;
    bool isBuy = true;
    SecurityType securityType = SecurityType::CorporateBond;
    double accruedInterest = 500.0;
    Currency currency = Currency::USD;

    Trade buyTrade {
        securityName,
        price,
        quantity,
        isBuy,
        securityType,
        accruedInterest,
        currency
    };

    String sellSecurityName = "CorpBond123";
    double sellPrice = 990.0;
    unsigned int sellQuantity = 20;
    bool isSell = false;
    SecurityType sellSecurityType = SecurityType::CorporateBond;
    double sellAccruedInterest = 200.0;
    Currency sellCurrency = Currency::USD;

    Trade sellTrade {
        sellSecurityName,
        sellPrice,
        sellQuantity,
        isSell,
        sellSecurityType,
        sellAccruedInterest,
        sellCurrency
    };
}



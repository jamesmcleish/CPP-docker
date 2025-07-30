1. Calculate pre-trade metrics
<hr>

2. Pre bucket trades into long short pairs for pre optimisation enhancement.

Possible improvement step

```
    1. Split trades into:
    - Long Corporate
    - Short Corporate
    - Long Government
    - Short Government

    2. For each security type:
    a. Group trades by Security Name.
    b. Greedy match opposite side trades with same Security Name by quantity.
    c. Collect matched buckets and residual unmatched trades.

    3. For residual unmatched trades:
    a. Construct cost matrix of DV01, duration, and type, and risk profile differences between longs and shorts.
    b. Solve the assignment problem to minimize total mismatch.
    c. Match quantities accordingly to form optimal buckets.
    d. Residual unmatched trades remain.

    Output: List of Buckets ready for downstream processing.
```
Cost matrix explained:
    Taking the remaining trades after step two split them into two vectors,
    long = {l1,l2,...,lp}
    short = {s1,s2,...,sp}
    If p < q or q < p add dummy trades to the shorter vector to make p = q.
    Then from these two vectors create the cost p x q cost matrix C where C[i,j] is the 'cost' of matching long trade li with short trade sj.

    To get C[i,j] entries we will create some weights for 


Make timeline for project goals if you were doing it at company
Ask questions throughout the project for them


I thought about first checking hard constraints, then running the trades through the optimised constraints. I realised that to check the hard constraints alone is tricky because you need to combine all trades in all ways in all accounts, even splitting single trades into parts.

Then I thought to do pre optimisation buckets where I match trades together likely to have little effect on metrics when combined. To do this I thought of creating a cost matrix using long vs short with optimisation weights (of my own choosing) for things like duration, dv01, same type (corporate vs. government) credit penalty.

I then realised that this bucketing while it would be good for passing hard constraints, it would later impact my ablity to apply optimisation priorities well (defeating a large part of the point of the exercise).

I considered some more options.

1. Pre-bucket multiple times, then test optimisation priorities against these pre bucket combinations to try to come up with a good solution that wasn't too computationally expensive but with penalties on the optimisation front.

2. Flip the order, optimise first come up with possible solutions and then test against hard constraints. This seems like a good way of going about this for the most 'optimised' solution. The problem here is that the algorithm may find highly 'optimal' solutions and yet be so far out of the hard constraints that it will not be possible to place the trades within some or any accounts.

3. A single algorithm that handles all steps simulataneously. Mixed integer linear programming solutions can be computationally expensive and tricky to implement but it seemed like a decent option here. The problems here are possible no solutions and the solution taking too long to be reached. Using google OR tools linear solver package we can 






James rewrite:

1. First, pre-bucket trades by grouping longs and shorts with similar risk profiles—using metrics like duration, DV01, and corporate vs government classification—to create clusters likely to offset each other’s risk. Then, apply hard constraint checks on these buckets across all accounts, but instead of fixing which bucket goes to which account, generate multiple feasible assignment options for each bucket. This maintains flexibility in how buckets are allocated later. Finally, run an optimization process that takes these candidate account assignments and applies the investment team’s optimization priorities (such as minimizing financing costs or balancing risk), selecting the best overall allocation. This approach reduces problem complexity while still allowing optimization to adjust allocations effectively.

2. Alternatively, skip the pre-bucketing step and keep trades separate. Before assigning trades, perform a thorough feasibility analysis to identify which accounts can accommodate which trades, based on hard constraints and existing account positions. Instead of assigning trades one by one, use a greedy heuristic that considers all trades collectively: it tries to place trades in accounts in a way that balances feasibility and overall fit, possibly splitting trades across accounts when needed to improve fit. This heuristic iteratively assigns trades by evaluating the impact on constraints globally rather than in isolation, thus avoiding local assignment errors. After building this set of plausible trade-to-account assignments, you then run an optimization that incorporates the specified priorities to refine and finalize the allocation. This method keeps maximum flexibility for optimization, while leveraging a heuristic to prune infeasible assignments upfront, but it requires careful design to avoid missing good solutions.

3. Lastly, a fully integrated approach can be used where hard constraints and optimization priorities are combined into one single model. This model represents all trades, accounts, and their constraints simultaneously, allowing trades (and even parts of trades) to be assigned across accounts in a way that strictly respects all hard constraints and optimizes the priority objectives. Using a solver such as MILP, the entire problem is solved at once without separate bucketing or heuristics. This method delivers the most precise and theoretically optimal solution, but it can be computationally intensive especially as the number of trades and accounts grows. It requires sophisticated modeling and powerful solvers but avoids the risk of losing solution quality due to early simplifications.

```
class Trade:
    def __init__(self, trade_id: str, side: str, security_name: str, security_type: str, quantity: float, price: float, accrued_interest: float):
        pass  # Initialize trade attributes


class Account:
    def __init__(self, account_id: str, initial_positions: dict, initial_cash: float, constraints: dict, metrics: dict):
        pass  # Initialize account details


class MetricCalculator:
    def __init__(self, accounts: list[Account], trades: list[Trade]):
        pass
    
    def calculate_metrics(self, trade_allocations: dict) -> dict:
        """
        Calculate metrics for each account given trade allocations.
        
        :param trade_allocations: Dict[trade_id][account_id] = quantity allocated
        :return: Dict[account_id][metric_name] = metric_value
        """
        pass


class ConstraintChecker:
    def __init__(self, accounts: list[Account], constraints: dict):
        pass
    
    def check_constraints(self, metrics: dict) -> bool:
        """
        Check if all accounts satisfy their constraints.
        
        :param metrics: Output of MetricCalculator.calculate_metrics
        :return: True if all constraints satisfied, else False
        """
        pass


class OptimizationProblem:
    def __init__(self, accounts: list[Account], trades: list[Trade], optimization_weights: dict):
        pass
    
    def setup_variables(self):
        """
        Create decision variables for trade allocations to accounts.
        """
        pass
    
    def setup_constraints(self):
        """
        Encode hard constraints and logical requirements into the solver.
        """
        pass
    
    def setup_objective(self):
        """
        Define the weighted sum objective combining all optimization priorities.
        """
        pass
    
    def solve(self) -> dict:
        """
        Run the solver to find optimal trade allocations.
        
        :return: Dict[trade_id][account_id] = quantity allocated
        """
        pass


class TradeAllocator:
    def __init__(self, accounts: list[Account], trades: list[Trade], optimization_weights: dict):
        self.metric_calculator = MetricCalculator(accounts, trades)
        self.constraint_checker = ConstraintChecker(accounts, accounts[0].constraints)  # Simplified for example
        self.optimization_problem = OptimizationProblem(accounts, trades, optimization_weights)
    
    def allocate_trades(self) -> dict:
        """
        End-to-end process:
        1. Setup and solve optimization problem.
        2. Validate constraints.
        3. Return trade allocation results.
        
        :return: Dict[trade_id][account_id] = quantity allocated
        """
        allocations = self.optimization_problem.solve()
        metrics = self.metric_calculator.calculate_metrics(allocations)
        
        if not self.constraint_checker.check_constraints(metrics):
            raise Exception("Constraints violated in solution")
        
        return allocations
```


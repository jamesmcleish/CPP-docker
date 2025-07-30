<h1>🔄 Prime Broker Trade Assignment Engine</h1>

<p>
This document outlines a full solution design for assigning a batch of fixed-income trades to Prime Brokerage (PB) accounts under hard constraints and optimization priorities. The process ensures constraint compliance while attempting to minimize business-critical metrics like financing cost.
</p>

<hr/>

<h2>🚩 Problem Context</h2>

<p>
In managing multiple PB accounts, each trade must be assigned to an account that meets all risk and compliance constraints. Accounts also differ in how they finance trades, which affects cost and exposure. Our objective is to find an assignment that:
</p>
<ul>
  <li>Satisfies <strong>all hard constraints</strong> (e.g., positive equity, leverage ratio caps)</li>
  <li>Optimizes account metrics (e.g., total financing cost) according to input priorities</li>
</ul>

<p>
Constraints vary by account and metric; optimization priorities are also account-specific. This makes the problem an instance of <strong>constrained multi-objective trade assignment</strong>.
</p>

<hr/>

<h2>📥 Inputs</h2>

<ul>
  <li><strong>Trades</strong> (0 to 200 per batch)
    <ul>
      <li>Side (buy/sell)</li>
      <li>Security Name and Type (corporate or government bond)</li>
      <li>Quantity, Price, Accrued Interest</li>
    </ul>
  </li>
  <li><strong>Accounts</strong> (2–5)
    <ul>
      <li>Initial positions and current metrics</li>
      <li>Per-account metric definitions (1–20 metrics)</li>
      <li>Metric coefficients <code>ω<sub>a,m,s</sub></code> (weight by security/account/metric)</li>
      <li>Hard constraints <code>C<sub>a,m</sub></code> (optional)</li>
      <li>Optimization priorities <code>OP<sub>a,m</sub></code> (optional)</li>
    </ul>
  </li>
  <li><strong>Configuration</strong>
    <ul>
      <li>Objective weight map: user-defined prioritization of metrics</li>
      <li>Solver timeout (e.g., 10 seconds)</li>
    </ul>
  </li>
</ul>

<h2>📤 Outputs</h2>

<ul>
  <li>Account assignment for each trade</li>
  <li>Final metric values for each account</li>
  <li>Solver status (optimal, feasible, or failed)</li>
</ul>

<hr/>

<h2>🧠 Algorithm Design</h2>

<h3>🎯 Goals</h3>
<ol>
  <li><strong>Primary:</strong> Ensure all metric-based constraints are satisfied</li>
  <li><strong>Secondary:</strong> Optimize metric scores according to investment team priorities</li>
</ol>

<h3>💡 Rationale for Optimization Model</h3>
<p>
The assignment problem is best formulated as a <strong>Mixed Integer Program (MIP)</strong>, solved using <a href="https://developers.google.com/optimization">Google OR-Tools</a>. MIPs allow for binary decision variables (assign trade to account), hard linear constraints (per-metric), and objective functions over weighted sums.
</p>

<h3>📉 Metric Computation</h3>

<pre><code>M<sub>a,m</sub> = ∑ x<sub>i,a</sub> × V<sub>i</sub> × ω<sub>a,m,s(i)</sub></code></pre>

<ul>
  <li><code>x<sub>i,a</sub> ∈ {0,1}</code>: trade <code>i</code> assigned to account <code>a</code></li>
  <li><code>V<sub>i</sub></code>: trade value (price × quantity ± accrued interest)</li>
  <li><code>ω<sub>a,m,s</sub></code>: metric sensitivity per account/metric/security</li>
</ul>

<h3>✅ Constraints</h3>

<pre><code>
M<sub>a,m</sub> ≤ C<sub>a,m</sub>  (if upper bound exists)
M<sub>a,m</sub> ≥ C<sub>a,m</sub>  (if lower bound exists)
</code></pre>

<h3>🏆 Objective</h3>

<pre><code>
maximize ∑ OP<sub>a,m</sub> × M<sub>a,m</sub>
</code></pre>

Only <em>unconstrained</em> metrics with non-zero priorities are used in the objective.

<h3>🔁 Fallback Strategy</h3>
<ol>
  <li>Try full optimization (all constraints and objectives)</li>
  <li>If infeasible or timeout, remove objectives → solve only for feasibility</li>
</ol>

<hr/>

<h2>🧱 Code & Architecture</h2>

<ul>
  <li><code>build_model(trades, accounts, constraints, priorities)</code>: returns OR-Tools solver and variable objects</li>
  <li><code>solve_model(solver)</code>: attempts full solve with timeout</li>
  <li><code>fallback_feasibility(solver)</code>: strips objective and retries</li>
  <li><code>extract_solution(x)</code>: maps binary variables to output assignments</li>
</ul>

<h3>📦 Modular Components</h3>
<ul>
  <li>📊 <strong>Trade Loader:</strong> parses trade files and builds position values</li>
  <li>📐 <strong>Metric Engine:</strong> computes all per-account metric adjustments</li>
  <li>⚙️ <strong>Constraint Manager:</strong> generates constraints and prioritizes objectives</li>
  <li>🧮 <strong>OR-Tools Solver:</strong> runs optimization and fallback</li>
  <li>📤 <strong>Reporter:</strong> outputs assignment, metrics, and diagnostics</li>
</ul>

<hr/>

<h2>🚦 Boundary Conditions</h2>

<ul>
  <li>0 trades: return current metrics, no changes</li>
  <li>Conflicting constraints: solver will return infeasible (log and alert)</li>
  <li>Multiple metrics apply to same trade in same account (aggregated correctly)</li>
</ul>

<hr/>

<h2>💼 Business Practices & Longevity</h2>

<h3>🔍 Monitoring</h3>
<ul>
  <li>Audit logs of solver inputs/outputs</li>
  <li>Feasibility fallback logs and alerts</li>
  <li>Daily constraint breach tracking</li>
</ul>

<h3>🧪 Testing</h3>
<ul>
  <li>Unit tests for each component</li>
  <li>Scenario-based validation (e.g., all buys, all sells, constraint edge cases)</li>
</ul>

<h3>📈 Operationalization</h3>
<ul>
  <li>Run as a daily batch or triggered pipeline (e.g., Airflow or GitHub Actions)</li>
  <li>Dry-run mode for traders to preview impact</li>
  <li>Configurable via YAML or JSON files for constraints, weights, and limits</li>
</ul>

<h3>🔄 Maintenance</h3>
<ul>
  <li>Modular code to support adding new metrics or security types</li>
  <li>Periodic review of metric weightings with investment team</li>
</ul>

<hr/>

<h2>🧠 Pseudocode Summary</h2>

<pre><code>
for each trade i:
    for each account a:
        calculate V<sub>i</sub> based on trade data
        for each metric m in a:
            delta_M<sub>a,m</sub> += x<sub>i,a</sub> * V<sub>i</sub> * ω<sub>a,m,s</sub>

create binary vars x<sub>i,a</sub> ∈ {0, 1}
add constraint: ∑ x<sub>i,a</sub> over a = 1 (each trade assigned once)

for all constrained metrics M<sub>a,m</sub>:
    add constraint M<sub>a,m</sub> ≤ or ≥ bound

objective = ∑ OP<sub>a,m</sub> × M<sub>a,m</sub>  (only unconstrained metrics)

try solver.Solve()
if failed or timeout:
    remove objective
    try solver.Solve()
</code></pre>

<hr/>

<h2>📌 Conclusion</h2>

<p>
This design ensures compliance with Prime Broker constraints while minimizing business costs. It is modular, auditable, and robust to scale and input volatility. Through clear separation of trade logic, metric management, and optimization, it supports future adaptability and reliability in live operations.
</p>

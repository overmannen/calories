import { useQuery } from "@tanstack/react-query";
import { fetchAthletes } from "../ahtletes";
import { motion, LayoutGroup } from "framer-motion";

export const Leaderboard = () => {
  const {
    data: athletes = [],
    isLoading,
    error,
  } = useQuery({
    queryKey: ["athletes"],
    queryFn: fetchAthletes,
  });

  if (isLoading) return <p className="loading-message">Laster leaderboard…</p>;
  if (error) return <p className="error-message">Noe gikk galt 😬</p>;

  const sorted = [...athletes].sort((a, b) => b.calories - a.calories);
  const maxBarHeight = 60; // 60% of container height

  return (
    <div className="leaderboard-section">
      <h2 className="leaderboard-title">🏆 Poengtavle</h2>

      <LayoutGroup>
        <div className="leaderboard-chart">
          <div className="goal-line" />

          {sorted.map((a) => {
            const heightPercent = (a.calories / 10000) * maxBarHeight;
            const isLeader = a.calories >= 10000;

            return (
              <motion.div
                key={a.name}
                layout
                transition={{ duration: 0.6, ease: "easeOut" }}
                className="bar-container"
              >
                <motion.span
                  initial={{ opacity: 0 }}
                  animate={{ opacity: 1 }}
                  className="bar-value"
                >
                  {a.calories}
                </motion.span>

                <motion.div
                  initial={{ height: 0 }}
                  animate={{ height: `${heightPercent}%` }}
                  transition={{ duration: 0.8, ease: "easeOut" }}
                  className={`bar ${isLeader ? "bar-leader" : "bar-default"}`}
                  style={{ height: `${heightPercent}%` }}
                />

                <span className="bar-name">{a.name}</span>
              </motion.div>
            );
          })}
        </div>
      </LayoutGroup>
    </div>
  );
};

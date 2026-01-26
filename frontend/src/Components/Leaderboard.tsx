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

  if (isLoading) return <p>Laster leaderboard…</p>;
  if (error) return <p>Noe gikk galt 😬</p>;

  const sorted = [...athletes].sort((a, b) => b.calories - a.calories);

  return (
    <div style={{ marginTop: 32 }}>
      <h2 style={{ marginBottom: 16 }}>🏆 Poengtavle</h2>

      <LayoutGroup>
        <div
          style={{
            position: "relative",
            display: "flex",
            alignItems: "flex-end",
            gap: 24,
            height: 300,
            padding: 16,
            borderRadius: 12,
            background: "#f8fafc",
          }}
        >
          {/* Mållinje på 10 000 */}
          <div
            style={{
              position: "absolute",
              left: 0,
              right: 0,
              bottom: 16 + 240,
              borderTop: "2px dashed #eab308",
              opacity: 0.9,
            }}
          />
          {sorted.map((a) => {
            const height = (a.calories / 10000) * 240;

            return (
              <motion.div
                key={a.name}
                layout
                transition={{ duration: 0.6, ease: "easeOut" }}
                style={{
                  display: "flex",
                  flexDirection: "column",
                  alignItems: "center",
                  width: 60,
                }}
              >
                {/* Verdi */}
                <motion.span
                  initial={{ opacity: 0 }}
                  animate={{ opacity: 1 }}
                  style={{
                    fontSize: 14,
                    marginBottom: 6,
                    fontWeight: 500,
                  }}
                >
                  {a.calories}
                </motion.span>

                {/* Søyle */}
                <motion.div
                  animate={{ height }}
                  transition={{ duration: 0.8, ease: "easeOut" }}
                  style={{
                    width: "100%",
                    background:
                      a.calories >= 10000
                        ? "#eac005" // leder
                        : "#4f46e5",
                    borderRadius: 8,
                  }}
                />

                {/* Navn */}
                <span
                  style={{
                    marginTop: 8,
                    fontSize: 14,
                    fontWeight: 500,
                    textAlign: "center",
                  }}
                >
                  {a.name}
                </span>
              </motion.div>
            );
          })}
        </div>
      </LayoutGroup>
    </div>
  );
};

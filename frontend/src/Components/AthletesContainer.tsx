import { Athletes } from "./Athletes";
import { useMutation, useQueryClient } from "@tanstack/react-query";
import { handleAddAthlete } from "../ahtletes";
import { Leaderboard } from "./Leaderboard";

export const AthletesContainer = () => {
  const queryClient = useQueryClient();

  const addAthleteMutation = useMutation({
    mutationFn: handleAddAthlete,
    onSuccess: () => {
      queryClient.invalidateQueries({ queryKey: ["athletes"] });
    },
  });

  const handleSubmit = (e: React.FormEvent<HTMLFormElement>) => {
    e.preventDefault();
    const formData = new FormData(e.currentTarget);
    const name = formData.get("name") as string;

    addAthleteMutation.mutate(name);
    e.currentTarget.reset();
  };

  return (
    <div className="athletes-container">
      <div className="left-column">
        <form onSubmit={handleSubmit} className="add-athlete-form">
          <input
            type="text"
            name="name"
            placeholder="Navn på utøver..."
            required
          />
          <button
            type="submit"
            disabled={addAthleteMutation.isPending}
            className="btn-primary"
          >
            {addAthleteMutation.isPending ? "Legger til..." : "Legg til utøver"}
          </button>
        </form>
        <Athletes />
      </div>
      <Leaderboard />
    </div>
  );
};

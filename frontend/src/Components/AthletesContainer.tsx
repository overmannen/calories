import { Athletes } from "./Athletes";
import { useMutation, useQueryClient } from "@tanstack/react-query";
import { handleAddAthlete } from "../ahtletes";

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
    <div>
      <form onSubmit={handleSubmit}>
        <input type="text" name="name" required />
        <button type="submit" disabled={addAthleteMutation.isPending}>
          {addAthleteMutation.isPending ? "Legger til..." : "Legg til utøver"}
        </button>
      </form>
      <Athletes />
    </div>
  );
};

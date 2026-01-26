import { useMutation, useQuery, useQueryClient } from "@tanstack/react-query";
import {
  fetchAthletes,
  handleDeleteAthlete,
  handleUpdateCalories,
} from "../ahtletes";
import { useState } from "react";

export type AthleteType = {
  id: number;
  name: string;
  calories: number;
};

export const Athletes = () => {
  const queryClient = useQueryClient();
  const [inputCalories, setInputCalories] = useState<{ [key: number]: string }>(
    {},
  );

  const {
    data: athletes = [],
    isLoading,
    error,
  } = useQuery({
    queryKey: ["athletes"],
    queryFn: fetchAthletes,
  });

  const handleUpdateCaloriesMutation = useMutation({
    mutationFn: ({ id, calories }: { id: number; calories: string }) =>
      handleUpdateCalories(id, calories),
    onSuccess: (_, variables) => {
      queryClient.invalidateQueries({ queryKey: ["athletes"] });
      setInputCalories((prev) => ({ ...prev, [variables.id]: "" }));
    },
  });

  const handleDeleteAthleteMutation = useMutation({
    mutationFn: handleDeleteAthlete,
    onSuccess: () => {
      queryClient.invalidateQueries({ queryKey: ["athletes"] });
    },
  });

  const handleInputChange = (id: number, value: string) => {
    setInputCalories((prev) => ({
      ...prev,
      [id]: value,
    }));
  };

  const handleCaloriesUpdate = (id: number) => {
    const calories = inputCalories[id];

    if (!calories || calories.trim() === "") {
      console.warn("Vennligst skriv inn antall kalorier");
      return;
    }

    handleUpdateCaloriesMutation.mutate({ id, calories });
  };

  if (isLoading) return <p>Laster utøvere...</p>;
  if (error) return <p>Noe gikk galt</p>;

  const sortedAthletes = athletes.sort((a, b) => a.id - b.id);

  return (
    <div>
      <h1>Utøvere</h1>
      <ul>
        {sortedAthletes.map((a) => (
          <li key={a.id}>
            <strong>{a.name}</strong>: {a.calories} kcal{" "}
            <input
              type="number"
              placeholder="Legg til kalorier"
              value={inputCalories[a.id] || ""}
              onChange={(e) => handleInputChange(a.id, e.target.value)}
            />
            <button
              onClick={() => handleCaloriesUpdate(a.id)}
              disabled={handleUpdateCaloriesMutation.isPending}
            >
              {handleUpdateCaloriesMutation.isPending
                ? "Lagrer..."
                : "Oppdater"}
            </button>
            <button onClick={() => handleDeleteAthleteMutation.mutate(a.id)}>
              Slett utøver
            </button>
          </li>
        ))}
      </ul>
    </div>
  );
};

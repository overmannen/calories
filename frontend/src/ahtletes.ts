export type AthleteType = {
  id: number;
  name: string;
  calories: number;
};

export const fetchAthletes = async (): Promise<AthleteType[]> => {
  try {
    const response = await fetch("/api/athletes");
    if (!response.ok) {
      throw new Error(`Response status: ${response.status}`);
    }
    const data = await response.json();
    return data.athletes;
  } catch (error) {
    console.error(error);
    return [];
  }
};

export const handleAddAthlete = async (name: string) => {
  const response = await fetch("/api/add_athlete", {
    method: "POST",
    headers: {
      "Content-Type": "application/x-www-form-urlencoded",
    },
    body: `name=${encodeURIComponent(name)}`,
  });

  if (!response.ok) {
    throw new Error("Kunne ikke legge til utøver");
  }
};

export const handleUpdateCalories = async (id: number, calories: string) => {
  const response = await fetch(`/api/athlete/${id}`, {
    method: "PATCH",
    headers: {
      "Content-Type": "application/x-www-form-urlencoded",
    },
    body: `calories=${encodeURIComponent(calories)}`,
  });
  if (!response.ok) {
    throw new Error("Kunne ikke oppdatere utøver");
  }
};

export const handleDeleteAthlete = async (id: number) => {
  const response = await fetch(`/api/athlete/${id}`, {
    method: "DELETE",
    headers: {
      "Content-Type": "application/x-www-form-urlencoded",
    },
  });
  if (!response.ok) {
    throw new Error("Kunne ikke oppdatere utøver");
  }
};

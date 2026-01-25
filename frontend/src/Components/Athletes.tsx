import { useQuery } from "@tanstack/react-query";
import { fetchAthletes } from "../ahtletes";

export type AthleteType = {
  id: number;
  name: string;
  calories: number;
};

export const Athletes = () => {
  const {
    data: athletes = [],
    isLoading,
    error,
  } = useQuery({
    queryKey: ["athletes"],
    queryFn: fetchAthletes,
  });

  if (isLoading) return <p>Laster utøvere...</p>;
  if (error) return <p>Noe gikk galt 😬</p>;

  return (
    <div>
      <h1>Utøvere</h1>
      <ul>
        {athletes.map((a) => (
          <li key={a.id}>
            {a.name}:{a.calories}
          </li>
        ))}
      </ul>
    </div>
  );
};

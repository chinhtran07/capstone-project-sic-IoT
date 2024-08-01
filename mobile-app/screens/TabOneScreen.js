import { StyleSheet } from "react-native";

import { Text, View } from "../components/Themed";
import { SensorCard } from "../components/SensorCard";

export default function TabOneScreen() {
  const temperature = 23;
  const humidity = 65;
  const soilMoisture = 45;

  return (
    <View style={styles.container}>
      <View>
        <SensorCard title="Temperature" value={temperature} unit="°C" />
        <SensorCard title="Humidity" value={humidity} unit="%" />
        <SensorCard title="Soil Moisture" value={soilMoisture} unit="%" />
        </View>
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    justifyContent: "center",
    alignItems: "center",
    backgroundColor: "#fff",
    padding: 16,
  },
  cardContainer: {
    flexDirection: "column",
    justifyContent: "space-between",
    width: "100%",
    maxWidth: 900, // Tùy chỉnh độ rộng tối đa cho card container
  },
});

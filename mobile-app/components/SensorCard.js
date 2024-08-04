import { Ionicons } from "@expo/vector-icons";
import { StyleSheet, Text, View } from "react-native";
import { Card } from "react-native-paper";

// Danh sách các biểu tượng cho các cảm biến
const ICONS = {
  temperature: "thermometer",
  humidity: "water",
  soilMoisture: "water-outline",
};

export const SensorCard = ({ title, value, unit }) => {
  const iconName = ICONS[title.toLowerCase()] || "information-circle"; // Sử dụng biểu tượng mặc định nếu không có trong danh sách

  return (
    <Card style={styles.card}>
      <Card.Content style={styles.cardContent}>
        <View style={styles.leftContainer}>
          <Ionicons name={iconName} size={24} color="black" />
          <Text style={styles.title}>{title}</Text>
        </View>
        <View style={styles.rightContainer}>
          <Text style={styles.valueText}>{value}</Text>
          <Text style={styles.unitText}>{unit}</Text>
        </View>
      </Card.Content>
    </Card>
  );
};

const styles = StyleSheet.create({
  card: {
    flex: 1,
    margin: 8,
    maxHeight: 100,
  },
  cardContent: {
    flexDirection: "row",
    justifyContent: "space-between",
    alignItems: "center",
    width: 370,
  },
  leftContainer: {
    flexDirection: "row",
    alignItems: "center",
  },
  rightContainer: {
    alignItems: "flex-end",
  },
  title: {
    fontSize: 18,
    marginLeft: 8,
  },
  valueText: {
    fontSize: 24,
    fontWeight: "bold",
  },
  unitText: {
    fontSize: 16,
    color: "gray",
  },
});

using System;
using System.IO;
using System.IO.Ports;
using System.Management;
using System.Collections.Generic;
using System.Text.Json;

namespace SerialPortExample
{
    class Program
    {
        public static void Main()
        {
            string chiniseCOMDevice = FindPortByDescription("USB-SERIAL CH340");

            if (!string.IsNullOrEmpty(chiniseCOMDevice))
            {
                // Update this path to the actual location of your ConfigFile.json
                string jsonFilePath = @"../config/ConfigFile.json"; // Full path or relative path

                UpdateConfigFile(chiniseCOMDevice, jsonFilePath);
            }
            else
            {
                Console.WriteLine("COM port not found.");
            }
        }

        static ManagementObject[] FindPorts()
        {
            try
            {
                ManagementObjectSearcher searcher = new ManagementObjectSearcher("root\\CIMV2", "SELECT * FROM Win32_PnPEntity");
                List<ManagementObject> objects = new List<ManagementObject>();

                foreach (ManagementObject obj in searcher.Get())
                {
                    objects.Add(obj);
                }
                return objects.ToArray();
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
                return new ManagementObject[] { };
            }
        }

        static string FindPortByDescription(string description)
        {
            foreach (ManagementObject obj in FindPorts())
            {
                try
                {
                    if (obj["Description"].ToString().ToLower().Equals(description.ToLower()))
                    {
                        string comName = ParseCOMName(obj);
                        if (comName != null)
                        {
                            return comName;
                        }
                    }
                }
                catch (Exception e)
                {
                    Console.WriteLine(e);
                }
            }
            return string.Empty;
        }

        static string ParseCOMName(ManagementObject obj)
        {
            string name = obj["Name"].ToString();
            int startIndex = name.LastIndexOf("(");
            int endIndex = name.LastIndexOf(")");

            if (startIndex != -1 && endIndex != -1)
            {
                name = name.Substring(startIndex + 1, endIndex - startIndex - 1);
                return name;
            }
            return null;
        }

        // Method to update the ArduinoCOMPort in the ConfigFile.json
        static void UpdateConfigFile(string comPort, string fileName)
        {
            try
            {
                // Read the existing config file
                string jsonString = File.ReadAllText(fileName);
                // Deserialize the JSON file to an object
                var config = JsonSerializer.Deserialize<ArduinoConfig>(jsonString);

                // Only update the ArduinoCOMPort, leave ArduinoBaudRate unchanged
                if (config?.ArduinoSettings != null)
                {
                    config.ArduinoSettings.COMPort = comPort;
                }

                // Serialize the updated object back to JSON with indents
                jsonString = JsonSerializer.Serialize(config, new JsonSerializerOptions { WriteIndented = true });

                // Write the updated JSON back to the file
                File.WriteAllText(fileName, jsonString);

                Console.WriteLine($"[INFO] COM port '{comPort}' has been written to the config file {fileName}");
            }
            catch (Exception e)
            {
                Console.WriteLine("Error while updating the config file: " + e.Message);
            }
        }
    }

    // Define a class to match the structure of the JSON file
    public class ArduinoConfig
    {
        public ArduinoSettings ArduinoSettings { get; set; }
    }

    public class ArduinoSettings
    {
        public string COMPort { get; set; }
    }
}

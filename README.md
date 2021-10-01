# Point_Cloud_Processing
A point cloud processing pipeline for identifying  important changes in shape or color. 

Each pair of point clouds represents the exact same place in the years 2016 and 2020.

The pipeline consists of: 
- Ground Removal using RANSAC / threshold in Z-coord.
- Clustering and noise / background removal with DBSCAN.
- Registration based on K-NN for identifying changes in shape.
- Analysis in HSV space  and use of chi square test for identifying important changes in the color of the item.
- A use of thresholds to classify Point clouds as (1.Changed 2.Added 3.Removed 4.Color changed).

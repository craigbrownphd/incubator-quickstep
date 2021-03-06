# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements.  See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership.  The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License.  You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied.  See the License for the
# specific language governing permissions and limitations
# under the License.

# Declare micro-libs:
add_library(quickstep_queryoptimizer_costmodel_CostModel ../../empty_src.cpp CostModel.hpp)
add_library(quickstep_queryoptimizer_costmodel_SimpleCostModel SimpleCostModel.cpp SimpleCostModel.hpp)
add_library(quickstep_queryoptimizer_costmodel_StarSchemaSimpleCostModel
            StarSchemaSimpleCostModel.cpp
            StarSchemaSimpleCostModel.hpp)

# Link dependencies:
target_link_libraries(quickstep_queryoptimizer_costmodel_CostModel
                      quickstep_queryoptimizer_physical_Aggregate
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_costmodel_SimpleCostModel
                      glog
                      quickstep_catalog_CatalogRelation
                      quickstep_catalog_CatalogRelationStatistics
                      quickstep_queryoptimizer_costmodel_CostModel
                      quickstep_queryoptimizer_physical_Aggregate
                      quickstep_queryoptimizer_physical_CrossReferenceCoalesceAggregate
                      quickstep_queryoptimizer_physical_FilterJoin
                      quickstep_queryoptimizer_physical_HashJoin
                      quickstep_queryoptimizer_physical_NestedLoopsJoin
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_queryoptimizer_physical_PhysicalType
                      quickstep_queryoptimizer_physical_Selection
                      quickstep_queryoptimizer_physical_SharedSubplanReference
                      quickstep_queryoptimizer_physical_Sort
                      quickstep_queryoptimizer_physical_TableGenerator
                      quickstep_queryoptimizer_physical_TableReference
                      quickstep_queryoptimizer_physical_TopLevelPlan
                      quickstep_queryoptimizer_physical_WindowAggregate
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_costmodel_StarSchemaSimpleCostModel
                      glog
                      quickstep_catalog_CatalogRelation
                      quickstep_catalog_CatalogRelationStatistics
                      quickstep_catalog_CatalogTypedefs
                      quickstep_expressions_aggregation_AggregateFunction
                      quickstep_expressions_aggregation_AggregationID
                      quickstep_queryoptimizer_costmodel_CostModel
                      quickstep_queryoptimizer_expressions_AggregateFunction
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ComparisonExpression
                      quickstep_queryoptimizer_expressions_ExprId
                      quickstep_queryoptimizer_expressions_ExpressionType
                      quickstep_queryoptimizer_expressions_ExpressionUtil
                      quickstep_queryoptimizer_expressions_LogicalAnd
                      quickstep_queryoptimizer_expressions_LogicalOr
                      quickstep_queryoptimizer_expressions_PatternMatcher
                      quickstep_queryoptimizer_expressions_Predicate
                      quickstep_queryoptimizer_physical_Aggregate
                      quickstep_queryoptimizer_physical_CrossReferenceCoalesceAggregate
                      quickstep_queryoptimizer_physical_FilterJoin
                      quickstep_queryoptimizer_physical_HashJoin
                      quickstep_queryoptimizer_physical_NestedLoopsJoin
                      quickstep_queryoptimizer_physical_PatternMatcher
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_queryoptimizer_physical_PhysicalType
                      quickstep_queryoptimizer_physical_Selection
                      quickstep_queryoptimizer_physical_SharedSubplanReference
                      quickstep_queryoptimizer_physical_Sort
                      quickstep_queryoptimizer_physical_TableGenerator
                      quickstep_queryoptimizer_physical_TableReference
                      quickstep_queryoptimizer_physical_TopLevelPlan
                      quickstep_queryoptimizer_physical_WindowAggregate
                      quickstep_types_NullType
                      quickstep_types_Type
                      quickstep_types_TypeID
                      quickstep_types_TypedValue
                      quickstep_utility_EqualsAnyConstant
                      quickstep_utility_Macros)

# Module all-in-one library:
add_library(quickstep_queryoptimizer_costmodel ../../empty_src.cpp CostModelModule.hpp)
target_link_libraries(quickstep_queryoptimizer_costmodel
                      quickstep_queryoptimizer_costmodel_CostModel
                      quickstep_queryoptimizer_costmodel_SimpleCostModel
                      quickstep_queryoptimizer_costmodel_StarSchemaSimpleCostModel)
